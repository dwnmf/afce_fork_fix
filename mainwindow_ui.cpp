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
#include <QMenu>
#include <QMenuBar>

void MainWindow::setupUi()
{
    QApplication::setWindowIcon(QIcon(":/images/icon.png"));
    createActions();
    createMenu();
    createToolBar();
    QWidget *body = new QWidget;
    QWidget *zoomPanel = new QWidget;
    zoomPanel->setMinimumHeight(18);
    saScheme = new AfcScrollArea();
    QPalette pal = saScheme->palette();
    pal.setColor(QPalette::Window, pal.color(QPalette::Base));
    saScheme->setPalette(pal);

    setCentralWidget(body);
    QVBoxLayout *bodyLayout = new QVBoxLayout;
    bodyLayout->addWidget(saScheme);
    bodyLayout->addWidget(zoomPanel);
    body->setLayout(bodyLayout);
    zoomSlider = new QSlider(Qt::Horizontal, zoomPanel);
    zoomLabel = new QLabel;
    QHBoxLayout *zoomLayout= new QHBoxLayout;
    zoomLayout->addStretch();
    zoomLayout->addWidget(zoomLabel);
    zoomLayout->addWidget(zoomSlider);
    zoomPanel->setLayout(zoomLayout);
    zoomSlider->setRange(1, 20);
    zoomSlider->setSingleStep(1);
    zoomSlider->setPageStep(10);
    connect(zoomSlider, SIGNAL(valueChanged(int)),this, SLOT(setZoom(int)));
    zoomSlider->setValue(4);
    connect(saScheme, SIGNAL(zoomStepped(int)), this, SLOT(shiftZoom(int)));
    connect(saScheme, SIGNAL(scrollStepped(int)), this, SLOT(shiftScrollY(int)));

    createToolbox();

    dockCode = new QDockWidget(this);
    dockCode->setObjectName("dock_code");
    dockCode->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::RightDockWidgetArea, dockCode);
    codeWidget = new QFrame;
    codeLanguage = new QComboBox;
    codeText = new QTextEdit;
    codeLabel = new QLabel;
    actCode->setCheckable(true);
    actCode->setChecked(dockCode->isVisible());
    connect(actCode, SIGNAL(triggered(bool)), dockCode, SLOT(setVisible(bool)));
    connect(dockCode, SIGNAL(visibilityChanged(bool)), actCode, SLOT(setChecked(bool)));

    //connect(dockCode, SIGNAL(visibilityChanged(bool)), this, SLOT(docCodeVisibilityChanged(bool)));

    codeWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

    codeLabel->setBuddy(codeLanguage);

    codeText->setFont(QFont("Courier New", 12));
    codeText->setLineWrapMode(QTextEdit::NoWrap);
    codeText->setReadOnly(true);


    QVBoxLayout * vbl = new QVBoxLayout;
    vbl->addWidget(codeLabel);
    vbl->addWidget(codeLanguage);
    vbl->addWidget(codeText);
    codeWidget->setLayout(vbl);
    dockCode->setWidget(codeWidget);

    helpWindow = new THelpWindow();
    helpWindow->setObjectName("help_window");
    helpWindow->setAllowedAreas(Qt::AllDockWidgetAreas);
    addDockWidget(Qt::RightDockWidgetArea, helpWindow);
    helpWindow->hide();

    actHelp->setCheckable(true);
    actHelp->setChecked(helpWindow->isVisible());

    connect(actHelp, SIGNAL(triggered(bool)), helpWindow, SLOT(setVisible(bool)));
    connect(helpWindow, SIGNAL(visibilityChanged(bool)), actHelp, SLOT(setChecked(bool)));


}

QToolButton * createToolButton(const QString & fileName)
{
    QToolButton *Result = new QToolButton;
    Result->setIconSize(QSize(32, 32));
    Result->setIcon(QIcon(fileName));
    Result->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Result->setAutoRaise(true);
    Result->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    Result->setCheckable(true);
    Result->setAutoExclusive(true);
    return Result;
}

void MainWindow::createToolbox()
{
    dockTools = new QDockWidget(this);
    dockTools->setObjectName("dock_tools");
    dockTools->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockTools->setMinimumWidth(150);
    addDockWidget(Qt::LeftDockWidgetArea, dockTools);
    connect(dockTools, SIGNAL(visibilityChanged(bool)), this, SLOT(docToolsVisibilityChanged(bool)));

    tbArrow = createToolButton(":/images/arrow.png");
    tbArrow->setChecked(true);
    tbProcess = createToolButton(":/images/simple.png");
    tbAssign = createToolButton(":/images/assign.png");
    tbIf = createToolButton(":/images/if.png");
    tbFor = createToolButton(":/images/for.png");
    tbWhilePre = createToolButton(":/images/while.png");
    tbWhilePost = createToolButton(":/images/until.png");
    tbIo = createToolButton(":/images/io.png");
    tbOu = createToolButton(":/images/ou.png");
    tbForCStyle = createToolButton(":/images/forc.png");

    connect(tbArrow, SIGNAL(pressed()), this, SLOT(slotToolArrow()));
    connect(tbProcess, SIGNAL(pressed()), this, SLOT(slotToolProcess()));
    connect(tbAssign, SIGNAL(pressed()), this, SLOT(slotToolAssign()));
    connect(tbIf, SIGNAL(pressed()), this, SLOT(slotToolIf()));
    connect(tbFor, SIGNAL(pressed()), this, SLOT(slotToolFor()));
    connect(tbWhilePre, SIGNAL(pressed()), this, SLOT(slotToolWhilePre()));
    connect(tbWhilePost, SIGNAL(pressed()), this, SLOT(slotToolWhilePost()));
    connect(tbIo, SIGNAL(pressed()), this, SLOT(slotToolIo()));
    connect(tbOu, SIGNAL(pressed()), this, SLOT(slotToolOu()));
    connect(tbForCStyle, SIGNAL(pressed()), this, SLOT(slotToolForCStyle()));

    toolsWidget = new QFrame;
    toolsWidget->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    QVBoxLayout *tl = new QVBoxLayout;
    tl->setSpacing(2);
    tl->addWidget(tbArrow);
    tl->addWidget(tbIo);
    tl->addWidget(tbOu);
    tl->addWidget(tbProcess);
    tl->addWidget(tbAssign);
    tl->addWidget(tbIf);
    tl->addWidget(tbFor);
    tl->addWidget(tbWhilePre);
    tl->addWidget(tbWhilePost);
    tl->addWidget(tbForCStyle);
    tl->addStretch();
    toolsWidget->setLayout(tl);
    dockTools->setWidget(toolsWidget);

    actTools->setCheckable(true);
    actTools->setChecked(dockTools->isVisible());
    connect(actTools, SIGNAL(triggered(bool)), dockTools, SLOT(setVisible(bool)));
    connect(dockTools, SIGNAL(visibilityChanged(bool)), actTools, SLOT(setChecked(bool)));


}

void MainWindow::retranslateUi()
{
    dockTools->setWindowTitle(tr("Tools"));
    tbArrow->setText(tr("Select"));
    tbProcess->setText(tr("Process"));
    tbAssign->setText(tr("Assign"));
    tbIf->setText(tr("If...then...else"));
    tbFor->setText(tr("FOR loop"));
    tbWhilePre->setText(tr("loop with pre-condition"));
    tbWhilePost->setText(tr("loop with post-condition"));
    tbIo->setText(tr("Input"));
    tbOu->setText(tr("Output"));
    tbForCStyle->setText(tr("FOR loop (C/C++)"));
    actExit->setText(tr("E&xit"));
    actExit->setStatusTip(tr("Exit from program"));
    actOpen->setText(tr("&Open..."));
    actOpen->setStatusTip(tr("Open saved file"));
    actSave->setText(tr("&Save"));
    actSave->setStatusTip(tr("Save changes"));
    actSaveAs->setText(tr("Save &as..."));
    actSaveAs->setStatusTip(tr("Save changes in a new file"));
    actExport->setText(tr("&Export to raster..."));
    actExport->setStatusTip(tr("Save the flowchart in a raster picture format"));
    actExportSVG->setText(tr("&Export to SVG..."));
    actExportSVG->setStatusTip(tr("Save the flowchart in a vector picture format"));
    actPrint->setText(tr("&Print..."));
    actPrint->setStatusTip(tr("To print"));
    actNew->setText(tr("&New"));
    actNew->setStatusTip(tr("Create a new project"));
    actUndo->setText(tr("&Undo"));
    actUndo->setStatusTip(tr("Undo the last operation"));
    actRedo->setText(tr("&Redo"));
    actRedo->setStatusTip(tr("Restore the last undone action"));
    actCut->setText(tr("Cu&t"));
    actCut->setStatusTip(tr("Cut the current selection"));
    actCopy->setText(tr("&Copy"));
    actCopy->setStatusTip(tr("Copy the current selection"));
    actPaste->setText(tr("&Paste"));
    actPaste->setStatusTip(tr("Paste"));
    actDelete->setText(tr("&Delete"));
    actDelete->setStatusTip(tr("Delete the current selection"));
    actHelp->setText(tr("&Help"));
    actHelp->setStatusTip(tr("Toggle Help window"));
    actAbout->setText(tr("&About"));
    actAbout->setStatusTip(tr("Information about authors"));
    actAboutQt->setText(tr("About &Qt"));
    actAboutQt->setStatusTip(tr("Information about Qt"));
    actTools->setText(tr("&Tools"));
    actTools->setStatusTip(tr("Toggle the tool panel"));
    actCode->setText(tr("&Source code"));
    actCode->setStatusTip(tr("Toggle the source code panel"));



    actExit->setShortcut(tr("Alt+X"));
    actOpen->setShortcut(tr("Ctrl+O"));
    actSave->setShortcut(tr("Ctrl+S"));
    actNew->setShortcut(tr("Ctrl+N"));
    actUndo->setShortcut(tr("Ctrl+Z"));
    actRedo->setShortcut(tr("Ctrl+Y"));
    actCut->setShortcut(tr("Ctrl+X"));
    actCopy->setShortcut(tr("Ctrl+C"));
    actPaste->setShortcut(tr("Ctrl+V"));
    actDelete->setShortcut(tr("Del"));
    actHelp->setShortcut(tr("F1"));
    actPrint->setShortcut(tr("Ctrl+P"));
    actTools->setShortcut(tr("F2"));
    actCode->setShortcut(tr("F3"));

    menuFile->setTitle(tr("&File"));
    menuEdit->setTitle(tr("&Edit"));
    menuHelp->setTitle(tr("&Help"));
    menuWindow->setTitle(tr("&View"));
    menuLanguage->setTitle(tr("&Language"));

    toolBar->setWindowTitle(tr("Standard"));
    dockCode->setWindowTitle(tr("Source code"));
    

    slotReloadGenerators();

    codeLabel->setText(tr("&Select programming language:"));

    if (!fileName.isEmpty())
        setWindowTitle(tr("%1 - Algorithm Flowchart Editor").arg(fileName));
    else
        setWindowTitle(tr("Algorithm Flowchart Editor"));
    helpWindow->setWindowTitle(tr("Help window"));
    helpWindow->textBrowser->setSearchPaths(QStringList() << "./help/"+QLocale().name() << "./help/en_US");
  #if defined(Q_WS_X11) or defined(Q_OS_LINUX)
    helpWindow->textBrowser->setSearchPaths(QStringList() << QString(PROGRAM_DATA_DIR) + "help/"+QLocale().name() << QString(PROGRAM_DATA_DIR) + "help/en_US");
  #endif
    helpWindow->textBrowser->reload();


}

void MainWindow::createMenu()
{
    menuFile = menuBar()->addMenu("");
    menuFile->addAction(actNew);
    menuFile->addAction(actOpen);
    menuFile->addSeparator();
    menuFile->addAction(actSave);
    menuFile->addAction(actSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actExport);
    menuFile->addAction(actExportSVG);
    menuFile->addSeparator();
    menuFile->addAction(actPrint);
    menuFile->addSeparator();
    menuFile->addSeparator();
    menuFile->addAction(actExit);
    actAbout->isChecked();

    menuEdit = menuBar()->addMenu("");
    menuEdit->addAction(actUndo);
    menuEdit->addAction(actRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actCut);
    menuEdit->addAction(actCopy);
    menuEdit->addAction(actPaste);
    menuEdit->addSeparator();
    menuEdit->addAction(actDelete);

    menuWindow = menuBar()->addMenu("");
    menuWindow->addAction(actTools);
    menuWindow->addAction(actCode);
    menuWindow->addSeparator();
    menuLanguage = menuWindow->addMenu(tr("&Language"));
    for (int i = 0; i < actLanguages.size(); ++i) {
        menuLanguage->addAction(actLanguages[i]);
    }

    menuHelp = menuBar()->addMenu("");
    menuHelp->addAction(actHelp);
    menuHelp->addSeparator();
    menuHelp->addAction(actAbout);
    menuHelp->addAction(actAboutQt);
}

void MainWindow::createToolBar()
{
    toolBar = addToolBar("");
    toolBar->setObjectName("standard_toolbar");
    toolBar->setIconSize(QSize(32,32));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    toolBar->addAction(actNew);
    toolBar->addAction(actOpen);
    toolBar->addAction(actSave);
    toolBar->addSeparator();
    toolBar->addAction(actUndo);
    toolBar->addAction(actRedo);
    toolBar->addSeparator();
    toolBar->addAction(actCut);
    toolBar->addAction(actCopy);
    toolBar->addAction(actPaste);
    toolBar->addSeparator();
    toolBar->addAction(actHelp);
    toolBar->addSeparator();
    toolBar->addAction(actTools);
    toolBar->addAction(actCode);
}


void MainWindow::docToolsVisibilityChanged(bool visible)
{
    actTools->setChecked(visible);
}

void MainWindow::docCodeVisibilityChanged(bool visible)
{
    actCode->setChecked(visible);
}
