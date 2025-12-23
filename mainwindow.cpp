/****************************************************************************
**                                                                         **
** Copyright (C) 2009-2014 Victor Zinkevich. All rights reserved.          **
** Contact: vicking@yandex.ru                                              **
**                                                                         **
** This file is part of the Algorithm Flowchart Editor project.            **
**                                                                         **
** This file may be used under the terms of the GNU                        **
** General Public License versions 2.0 or 3.0 as published by the Free     **
** Software Foundation and appearing in the file LICENSE included in       **
** the packaging of this file.                                             **
** You can find license at http://www.gnu.org/licenses/gpl.html            **
**                                                                         **
****************************************************************************/

#include "mainwindow.h"
#include <QtGui>
#include <QDir>
#include <QLocale>
#include <QSettings>
#include <QTranslator>
#include <QWidgetList>
#include "qflowchartstyle.h"

QString afceVersion()
{
    return PROGRAM_VERSION;
}


void AfcScrollArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragCandidate = false;
        m_isDragging = true;
        m_lastDragPos = event->globalPos();
        m_dragStartPos = m_lastDragPos;
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        event->accept();
        emit mouseDown();
    }
}

void AfcScrollArea::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        QPoint delta = event->globalPos() - m_lastDragPos;
        m_lastDragPos = event->globalPos();
        
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        
        if (hBar) hBar->setValue(hBar->value() - delta.x());
        if (vBar) vBar->setValue(vBar->value() - delta.y());
        
        event->accept();
    } else {
        QScrollArea::mouseMoveEvent(event);
    }
}

void AfcScrollArea::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    QFlowChart *chart = qobject_cast<QFlowChart *>(widget());
    if (chart) {
        chart->realignObjects();
    }
}

bool AfcScrollArea::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == widget()) {
        QFlowChart *chart = qobject_cast<QFlowChart *>(widget());
        if (!chart || chart->status() != QFlowChart::Selectable) {
            return QScrollArea::eventFilter(watched, event);
        }

        switch (event->type()) {
        case QEvent::MouseButtonPress: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                m_dragCandidate = true;
                m_lastDragPos = mouseEvent->globalPos();
                m_dragStartPos = mouseEvent->globalPos();
            }
            break;
        }
        case QEvent::MouseMove: {
            if (!m_dragCandidate) {
                break;
            }
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if ((mouseEvent->buttons() & Qt::LeftButton) == 0) {
                break;
            }

            if (!m_isDragging) {
                int threshold = QApplication::startDragDistance();
                if ((mouseEvent->globalPos() - m_dragStartPos).manhattanLength() < threshold) {
                    break;
                }
                m_isDragging = true;
                setCursor(Qt::ClosedHandCursor);
            }

            QPoint delta = mouseEvent->globalPos() - m_lastDragPos;
            m_lastDragPos = mouseEvent->globalPos();

            QScrollBar *hBar = horizontalScrollBar();
            QScrollBar *vBar = verticalScrollBar();

            if (hBar) hBar->setValue(hBar->value() - delta.x());
            if (vBar) vBar->setValue(vBar->value() - delta.y());

            return true;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                m_dragCandidate = false;
                if (m_isDragging) {
                    m_isDragging = false;
                    setCursor(Qt::OpenHandCursor);
                    return true;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    return QScrollArea::eventFilter(watched, event);
}

void AfcScrollArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        QScrollArea::mouseReleaseEvent(event);
    }
}

void AfcScrollArea::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    if((event->modifiers() & Qt::ControlModifier) != 0) {
        event->ignore();
        emit zoomStepped(delta / 120);
    }
    else {
        if((event->modifiers() & Qt::ShiftModifier) == 0) {
            QScrollBar * vsb = verticalScrollBar();
            if (vsb != nullptr) {
                /* scroll vertically */
                vsb->setValue(vsb->value() - delta);
            }
        }
        else {
            QScrollBar * gsb = horizontalScrollBar();
            if (gsb != nullptr) {
                /* scroll horizontally */
                gsb->setValue(gsb->value() - delta);
            }
        }
        event->accept();
    }
}


MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags), fDocument(nullptr)
{
#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
    QDir::setSearchPaths("generators", QStringList() << QString(PROGRAM_DATA_DIR) + "generators");
#else
    QDir::setSearchPaths("generators", QStringList() << qApp->applicationDirPath() + "/generators");
#endif

    setupUi();
    readSettings();
    retranslateUi();

    QFlowChart *fc = new QFlowChart(this);
    setDocument(fc);
    document()->setZoom(1);
    connect(document(), SIGNAL(statusChanged()), this, SLOT(slotStatusChanged()));
    connect(document(), SIGNAL(editBlock(QBlock *)), this, SLOT(slotEditBlock(QBlock *)));
    connect(actUndo, SIGNAL(triggered()), document(), SLOT(undo()));
    connect(actRedo, SIGNAL(triggered()), document(), SLOT(redo()));
    connect(document(), SIGNAL(changed()), this, SLOT(updateActions()));
    connect(document(), SIGNAL(changed()), this, SLOT(generateCode()));
    document()->setStatus(QFlowChart::Selectable);
    connect(saScheme, SIGNAL(mouseDown()), document(), SLOT(deselectAll()));
    connect(codeLanguage, SIGNAL(activated(int)), this, SLOT(codeLangChanged(int)));

    QFlowChartStyle st;
    QPalette pal = palette();
    st.setLineWidth(2);
    st.setNormalBackground(pal.color(QPalette::Base));
    st.setNormalForeground(pal.color(QPalette::WindowText));
    st.setNormalMarker(Qt::red);
    st.setSelectedBackground(pal.color(QPalette::Highlight));
    st.setSelectedForeground(pal.color(QPalette::HighlightedText));
    st.setNormalMarker(Qt::green);
    st.setFontSize(10);
    document()->setChartStyle(st);

    labelMenu = new QLabel(statusBar());

    statusBar()->setSizeGripEnabled(false);
    statusBar()->addWidget(labelMenu);
    labelMenu->setAlignment(Qt::AlignCenter);


    isSaved = true; //Let's allow to close application if no modification were made in empty document
    connect(document(), SIGNAL(modified()), SLOT(slotDocumentChanged()));
    connect(this, SIGNAL(documentLoaded()), SLOT(slotDocumentLoaded()));
    connect(this, SIGNAL(documentSaved()), SLOT(slotDocumentSaved()));

    if (qApp->arguments().size() > 1) {
        QFile test(qApp->arguments().at(1));
        if(test.exists()) {
            slotOpenDocument(qApp->arguments().at(1));
        }
        else {
            QMessageBox::critical(this, tr("Failed to open a file"), tr("Unable to open file '%1'.").arg(qApp->arguments().at(1)));
        }
    }
}


void MainWindow::writeSettings()
{
    QSettings settings("afce", "application");

    settings.setValue("geometry", geometry());
    settings.setValue("windowState", saveState());
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::readSettings()

{
    QSettings settings("afce", "application");

    setGeometry(settings.value("geometry", QRect(100, 100, 800, 600)).toRect());
    restoreState(settings.value("windowState").toByteArray());

}
void MainWindow::createActions()
{
    actExit = new QAction(QIcon(":/images/exit.png"), "", this);
    actOpen = new QAction(QIcon(":/images/open_document_32_h.png"), "", this);
    actNew = new QAction(QIcon(":/images/new_document_32_h.png"), "", this);
    actSave = new QAction(QIcon(":/images/save_32_h.png"), "", this);
    actSaveAs = new QAction(this);
    //actUndo = new QAction(QIcon(":/images/undo_32_h.png"), "", this);
    //actRedo = new QAction(QIcon(":/images/redo_32_h.png"), "", this);
    actUndo = new QAction(QIcon(":/images/restart-3.png"), "", this);
    actRedo = new QAction(QIcon(":/images/restart-4.png"), "", this);
    actCut = new QAction(QIcon(":/images/cut_clipboard_32_h.png"), "", this);
    actCopy = new QAction(QIcon(":/images/copy_clipboard_32_h.png"), "", this);
    actPaste = new QAction(QIcon(":/images/paste_clipboard_32_h.png"), "", this);
    actDelete = new QAction(QIcon(":/images/delete_x_32_h.png"), "", this);
    actExport = new QAction(this);
    actExportSVG = new QAction(this);
    actHelp = new QAction(QIcon(":/images/help-icon.png"), "", this);
    actAbout = new QAction(this);
    actAboutQt = new QAction(this);
    actPrint = new QAction(QIcon(":/images/print_32_h.png"), "", this);
    actTools = new QAction(QIcon(":/images/toolbar.png"), "", this);
    actCode = new QAction(QIcon(":/images/source-code.png"), "", this);


    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actNew, SIGNAL(triggered()), this, SLOT(slotFileNew()));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(slotFileOpen()));
    connect(actSave, SIGNAL(triggered()), this, SLOT(slotFileSave()));
    connect(actSaveAs, SIGNAL(triggered()), this, SLOT(slotFileSaveAs()));
    connect(actExport, SIGNAL(triggered()), this, SLOT(slotFileExport()));
    connect(actExportSVG, SIGNAL(triggered()), this, SLOT(slotFileExportSVG()));
    connect(actPrint, SIGNAL(triggered()), this, SLOT(slotFilePrint()));
    connect(actCut, SIGNAL(triggered()), this, SLOT(slotEditCut()));
    connect(actCopy, SIGNAL(triggered()), this, SLOT(slotEditCopy()));
    connect(actPaste, SIGNAL(triggered()), this, SLOT(slotEditPaste()));
    connect(actDelete, SIGNAL(triggered()), this, SLOT(slotEditDelete()));

//    connect(actHelp, SIGNAL(triggered()), this, SLOT(slotHelpHelp()));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(slotHelpAbout()));
    connect(actAboutQt, SIGNAL(triggered()), this, SLOT(slotHelpAboutQt()));

    /* ... */
//    connect(actTools, SIGNAL(triggered()), this, SLOT(slotTools()));


    QHash<QString, QString> avlLangs = enumLanguages();
    QList<QString> locales = avlLangs.keys();
    for (int i = 0; i < locales.size(); ++i) {
        QAction *act = new QAction(this);
        act->setText(avlLangs[locales[i]]);
        act->setData(locales[i]);
        actLanguages.append(act);
        connect(act, SIGNAL(triggered()), this, SLOT(slotChangeLanguage()));
    }

}


MainWindow::~MainWindow()
{
}

bool MainWindow::okToContinue()
{
    int r;
    if (!isSaved) {
        r = QMessageBox::warning(this,
                                 tr("Afce"), tr("There are unsaved changes. Do you really want to close afce?"),
                                 QMessageBox::Yes | QMessageBox::Default,
                                 QMessageBox::No | QMessageBox::Escape);

        if (r != QMessageBox::Yes) {
            return false;
        }
    }
    return true;
}

void MainWindow::slotStatusChanged()
{
    if(document())
    {
        if(document()->status() == QFlowChart::Selectable)
        {
            tbArrow->setChecked(true);
        }
    }
}

void MainWindow::updateActions()
{
    if(document())
    {
        if (actTools->isVisible())
        {
            actTools->setEnabled(false);
        }



        actCode->setEnabled(document()->status() != QFlowChart::Insertion);
        actTools->setEnabled(document()->status() != QFlowChart::Insertion);
        actHelp->setEnabled(document()->status() != QFlowChart::Insertion);
        actUndo->setEnabled(document()->canUndo() && document()->status() != QFlowChart::Insertion);
        actRedo->setEnabled(document()->canRedo() && document()->status() != QFlowChart::Insertion);
        actOpen->setEnabled(document()->status() != QFlowChart::Insertion);
        actSave->setEnabled(document()->status() != QFlowChart::Insertion);
        actSaveAs->setEnabled(document()->status() != QFlowChart::Insertion);
        actPrint->setEnabled(document()->status() != QFlowChart::Insertion);
        actExport->setEnabled(document()->status() != QFlowChart::Insertion);
        actExportSVG->setEnabled(document()->status() != QFlowChart::Insertion);
        actCopy->setEnabled(document()->status() == QFlowChart::Selectable && document()->activeBlock());
        actCut->setEnabled(document()->status() == QFlowChart::Selectable && document()->activeBlock());
        actPaste->setEnabled(document()->status() == QFlowChart::Selectable && document()->canPaste());
        actDelete->setEnabled(document()->status() == QFlowChart::Selectable && document()->activeBlock());
    }
    else
    {
        actUndo->setEnabled(false);
        actRedo->setEnabled(false);
        actOpen->setEnabled(false);
        actSave->setEnabled(false);
        actSaveAs->setEnabled(false);
        actPrint->setEnabled(false);
        actExport->setEnabled(false);
        actExportSVG->setEnabled(false);
        actCopy->setEnabled(false);
        actCut->setEnabled(false);
        actPaste->setEnabled(false);
        actDelete->setEnabled(false);
    }
}

void MainWindow::setDocument(QFlowChart * aDocument)
{
    if (fDocument) {
        fDocument->removeEventFilter(saScheme);
    }
    fDocument = aDocument;
    saScheme->setWidget(fDocument);
    saScheme->setAutoFillBackground(true);
    if (fDocument) {
        fDocument->installEventFilter(saScheme);
    }
    fDocument->show();
    fDocument->move(0,0);
}
void MainWindow::setZoom(int quarts)
{
    if(quarts > 20) quarts = 20;
    if(quarts < 1) quarts = 1;
    zoomLabel->setText(tr("Zoom: %1 %").arg(quarts * 25));
    if (document())
    {
        document()->setZoom(quarts * 25 / 100.0);
    }
}

void MainWindow::shiftZoom(int step)
{
    int z = zoomSlider->value();
    zoomSlider->setValue(z + step);
}

void MainWindow::shiftScrollY(int step)
{
    saScheme->viewport()->scroll(0, step);
}

void setApplicationLocale(const QString &localeName)
{
    QLocale locale(localeName);
    QLocale::setDefault(locale);
    static QTranslator *qtTranslator = NULL;
    static QTranslator *myappTranslator = NULL;

    if(!qtTranslator) {
        qApp->removeTranslator(qtTranslator);
        delete qtTranslator;
        qtTranslator = new QTranslator;
    }
    if(!myappTranslator) {
        qApp->removeTranslator(myappTranslator);
        delete myappTranslator;
        myappTranslator = new QTranslator;
    }

#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
    qtTranslator->load(locale, "qt", "_", QString(PROGRAM_DATA_DIR) + "locale", ".qm");
#else
    qtTranslator->load(locale, "qt", "_", qApp->applicationDirPath() + "/locale",  ".qm");
#endif
    qApp->installTranslator(qtTranslator);

#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
    myappTranslator->load("afce_" + locale.name() + ".qm", QString(PROGRAM_DATA_DIR) + "locale");
#else
    myappTranslator->load(locale, "afce", "_", qApp->applicationDirPath() + "/locale", ".qm");
#endif
    qApp->installTranslator(myappTranslator);

    qDebug() << "Detected system locale: " << QLocale::system().name();
    qDebug() << "Set locale: " << locale.name();
    qDebug() << qApp->applicationDirPath();
    QWidgetList wl = qApp->allWidgets();
    for(int i = 0; i < wl.size(); ++i) {
        wl[i]->setLocale(locale);
    }
}
