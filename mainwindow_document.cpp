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
#include "sourcecodegenerator.h"
#include <QtGui>
#include <QtSvg>
#include <QDir>
#include <QFileInfo>
#include <QImageWriter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QProcess>
#include <QRegExp>
#include <QSettings>
#include <QTextStream>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>

void MainWindow::slotFileOpen()
{
    QString fn = QFileDialog::getOpenFileName ( this,
                                                tr("Select a file to open"), "", tr("Algorithm flowcharts (*.afc)"));
    if(!fn.isEmpty())
    {

        if (!isSaved)
            if(QMessageBox::warning(this, tr("Unsaved changes"), tr("You are about to open another document. It will discard all unsaved changes in the current document."),
                                    QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
                return;
        slotOpenDocument(fn);

    }
}

void MainWindow::slotOpenDocument(const QString &fn) {
    emit documentUnloaded();
    fileName = fn;
    setWindowTitle(tr("%1 - Algorithm Flowchart Editor").arg(fileName));
    QFile xml(fileName);
    if (xml.exists())
    {
        xml.open(QIODevice::ReadOnly | QIODevice::Text);
        QDomDocument doc;
        if (doc.setContent(&xml, false))
        {
            document()->root()->setXmlNode(doc.firstChildElement());
            document()->setZoom(1);
        }
    }
    emit documentLoaded();

}

void MainWindow::slotFilePrint()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog pd(&printer, this);
    if (pd.exec() == QDialog::Accepted)
    {
        double oldZoom = document()->zoom();
        document()->setZoom(1);
        document()->setStatus(QFlowChart::Display);
        QBlock *r = document()->root();
        r->adjustSize(1);
        r->adjustPosition(0,0);
        QRect page = printer.pageRect();
        double z =  page.width()  / (double) r->width;
        if (r->height * z > page.height())
        {
            z = page.height() / (double) r->height;
        }
        if (z > (printer.resolution()/96.0)) z = printer.resolution()/96.0;
        document()->setZoom(z);
        r->adjustSize(z);
        r->adjustPosition(0, 0);
        QPainter canvas;
        canvas.begin(&printer);
        document()->paintTo(&canvas);
        canvas.end();
        document()->setZoom(oldZoom);
        document()->setStatus(QFlowChart::Selectable);
    }
}

void MainWindow::slotFileNew()
{
    QProcess::startDetached(QApplication::applicationFilePath());
}

void MainWindow::slotFileSave()
{
    if (fileName.isEmpty())
    {
        slotFileSaveAs();
    }
    else
    {
        QDomDocument doc = document()->document();
        QString xmlString = doc.toString(2);
        QFile xml(fileName);
        xml.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        QTextStream stream(&xml);
        stream.setCodec(QTextCodec::codecForName("utf-8"));
        stream << xmlString;
        xml.close();
        emit documentSaved();
    }
}

void MainWindow::slotFileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Select a file to save"), "", tr("Algorithm flowcharts (*.afc)"));
    if (!fn.isEmpty())
    {
        if(fn.right(4).toLower() != ".afc") fn += ".afc";
        fileName = fn;
        setWindowTitle(tr("%1 - Algorithm Flowchart Editor").arg(fileName));
        slotFileSave();
    }
}

void MainWindow::slotDocumentSaved() {
    isSaved = true;
}

void MainWindow::slotDocumentChanged() {
    isSaved = false;
}

void MainWindow::slotDocumentLoaded() {
    isSaved = true;
}

void MainWindow::slotChangeLanguage()
{
    QAction * action = (QAction *)sender();
    QString localeName = action->data().toString();
    setApplicationLocale(localeName);
    retranslateUi();

    QSettings settings("afce", "application");
    settings.setValue("locale", localeName);
}

void MainWindow::slotReloadGenerators()
{
    int i = codeLanguage->currentIndex();
    codeLanguage->clear();
    QDir gd("generators:");
    QStringList gens = gd.entryList(QStringList() << "*.json", QDir::Files, QDir::Name);

    for (int g = 0; g < gens.size(); ++g) {
        QFile f(gd.absoluteFilePath(gens[g]));
        QString lang_name = QFileInfo(f.fileName()).baseName();
        if(f.open(QIODevice::ReadOnly|QIODevice::Text)) {
            QJsonDocument json = QJsonDocument::fromJson(f.readAll());
            f.close();
            QJsonObject obj = json.object();
            QString loc = QLocale().name();
            if(obj.contains("name")) {
                QJsonObject jn = obj.value("name").toObject();
                if(jn.contains(loc)) {
                    lang_name = jn.value(loc).toString();
                }
                else if (jn.contains("en_US")) {
                    lang_name = jn.value("en_US").toString();
                }
            }
            codeLanguage->addItem(lang_name, QFileInfo(f.fileName()).baseName());
        }
    }

    if (i!=-1)
        codeLanguage->setCurrentIndex(i);
    else
        codeLanguage->setCurrentIndex(0);

}


void MainWindow::slotFileExport()
{
    QString filter = getWriteFormatFilter();
    QString sf = getFilterFor("png");
    QString fn = QFileDialog::getSaveFileName(this, tr("Select a file to export"), "", filter, &sf);
    if(!fn.isEmpty())
    {
        qDebug() << "Selected filter: " << sf;
        QRegExp rx("\\(\\*([^\\)]+)\\)$");
        QStringList masks;
        if(rx.indexIn(sf)!= -1) {
            // each filter may content several masks splitted by a spaces
            masks = rx.cap(1).split(" ", Qt::SkipEmptyParts);

            bool matches = false;
            for(int i = 0; i < masks.size(); ++i) {
                QString ex = masks.at(i).toLower();
                if(fn.toLower().endsWith(ex)) {
                    matches = true;
                    break;
                }
            }

            // if no extension or a wrong extension is added then the correct extension will be appended
            if(!matches && !masks.empty()) {
                fn += masks.first().toLower();
            }
        }
        double oldZoom = document()->zoom();
        document()->setZoom(1);
        document()->setStatus(QFlowChart::Display);
        QBlock *r = document()->root();
        r->adjustSize(1);
        r->adjustPosition(0,0);
        QImage img(r->width, r->height, QImage::Format_ARGB32_Premultiplied);
        img.fill(0);
        QPainter canvas(&img);
        canvas.setRenderHint(QPainter::Antialiasing);
        document()->paintTo(&canvas);
        img.save(fn);
        document()->setZoom(oldZoom);
        document()->setStatus(QFlowChart::Selectable);
    }
}

void MainWindow::slotFileExportSVG()
{
    QString filter = getWriteFormatFilter();
    QString fn = QFileDialog::getSaveFileName(this, tr("Select a file to export"), "", getFilterFor("svg"));
    if(!fn.isEmpty())
    {
        if(fn.right(4).toLower() != ".svg") fn += ".svg";
        double oldZoom = document()->zoom();
        document()->setZoom(1);
        document()->setStatus(QFlowChart::Display);
        QBlock *r = document()->root();
        r->adjustSize(1);
        r->adjustPosition(0,0);
        QSvgGenerator svg;
        svg.setSize(QSize(r->width, r->height));
        svg.setResolution(90);
        svg.setFileName(fn);
        QPainter canvas(&svg);
        canvas.setRenderHint(QPainter::Antialiasing);
        r->paint(&canvas, true);

        document()->setZoom(oldZoom);
        document()->setStatus(QFlowChart::Selectable);
    }
}


void MainWindow::slotEditCut()
{
    slotEditCopy();
    slotEditDelete();
}

void MainWindow::slotEditCopy()
{
    if(document())
    {
        if(document()->activeBlock())
        {
            QDomDocument doc("AFC"); // do not localize!
            QDomElement block = document()->activeBlock()->xmlNode(doc);
            if (document()->activeBlock()->isBranch)
            {
                QDomElement alg = doc.createElement("algorithm");
                alg.appendChild(block);
                doc.appendChild(alg);
            }
            else
            {
                if(block.nodeName() != "algorithm")
                {
                    QDomElement alg = doc.createElement("algorithm");
                    QDomElement branch = doc.createElement("branch");
                    alg.appendChild(branch);
                    branch.appendChild(block);
                    doc.appendChild(alg);
                }
                else
                    doc.appendChild(block);
            }
            QClipboard *clipbrd = QApplication::clipboard();
            clipbrd->setText(doc.toString(2));
            updateActions();
        }
    }
}

void MainWindow::slotEditPaste()
{
    if(document())
    {
        QClipboard *clipbrd = QApplication::clipboard();
        document()->setBuffer(clipbrd->text());
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }
}
void MainWindow::slotEditDelete()
{
    if(document())
    {
        if(document()->status() == QFlowChart::Selectable)
        {
            document()->deleteActiveBlock();
        }
    }
}


void MainWindow::codeLangChanged(int )
{
    generateCode();
}

void MainWindow::generateCode()
{
    if (document() && codeLanguage->currentIndex() >= 0)
    {
        SourceCodeGenerator gen;
        gen.loadRule("generators:" + codeLanguage->itemData(codeLanguage->currentIndex()).toString() + ".json");
        codeText->setText(gen.applyRule(document()->document()));
    }
}

QString MainWindow::getFilterFor(const QString & fileExt)
{
    return tr("%1 image (*.%2)").arg(fileExt.toUpper(), fileExt);
}

QString MainWindow::getWriteFormatFilter()
{
    QString result;
    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    for(int i = 0; i < formats.size(); ++i)
    {
        if(!result.isEmpty()) result.append(";;");
        result.append(getFilterFor(formats.at(i)));
    }
    return result;
}
QHash<QString, QString> MainWindow::enumLanguages()
{
    QHash<QString, QString> result;
#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
    QDir dir(QString(PROGRAM_DATA_DIR) + "locale");
#else
    QDir dir(qApp->applicationDirPath() + "/locale");
#endif

    QStringList qms = dir.entryList(QStringList() << "afce_*.qm", QDir::Files, QDir::Name);

    QRegExp rx("^afce_([-_a-zA-Z]+)\\.qm$");
    for (int i = 0; i < qms.size(); ++i) {
        if(rx.indexIn(qms[i]) >= 0) {
            QString localeName = rx.cap(1);
            QLocale loc(localeName);
            result.insert(localeName, loc.nativeLanguageName());
        }
    }
    return result;
}
