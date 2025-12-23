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
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

void MainWindow::slotHelpAbout()
{
    QDialog dlg;
    QPushButton *ok = new QPushButton(tr("&OK"));
    QLabel *text = new QLabel(tr("<html><h1>AFCE</h1><p>Algorithm Flowchart Editor. Version %0</p><p>Copyright 2008-2014 Viktor Zinkevich. All rights reserved.</p> \
<p>Contributors:  Sergey Ryabenko, Alexey Loginov</p> \
<p>The program is provided AS IS with NO WARRANTY OF ANY KIND,<br> INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND<br> \
FITNESS FOR A PARTICULAR PURPOSE.</p></html>").arg(PROGRAM_VERSION));
    QLabel *ico = new QLabel();
    ico->setPixmap(QPixmap(":/images/icon.png"));
    QGridLayout *layout = new QGridLayout;
    QHBoxLayout *bl = new QHBoxLayout;
    bl->addStretch();
    bl->addWidget(ok);
    layout->addWidget(ico, 0, 0, 2, 1, Qt::AlignTop);
    layout->addWidget(text, 0, 1);
    layout->addLayout(bl, 1, 1);
    dlg.setLayout(layout);
    connect(ok, SIGNAL(clicked()), &dlg, SLOT(accept()));
    dlg.exec();
}

void MainWindow::slotHelpAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::slotEditBlock(QBlock *aBlock)
{
    if(aBlock)
    {
        QDialog dlg;
        dlg.setSizeGripEnabled(true);
        dlg.setMinimumWidth(480);
        QFont gostFont("Tahoma");
        gostFont.setPointSize(10);
        QVBoxLayout *mainLayout = new QVBoxLayout();
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *btnOk = new QPushButton(tr("&OK"));
        QPushButton *btnCancel = new QPushButton(tr("&Cancel"));
        connect(btnOk, SIGNAL(clicked()), &dlg, SLOT(accept()));
        connect(btnCancel, SIGNAL(clicked()), &dlg, SLOT(reject()));
        buttonLayout->addStretch();
        buttonLayout->addWidget(btnOk);
        buttonLayout->addWidget(btnCancel);
        dlg.setLayout(mainLayout);
        if(aBlock->type() == "process" || aBlock->type() == "if" || aBlock->type() == "pre" || aBlock->type() == "post" )
        {
            dlg.setMinimumHeight(120);
            QLineEdit *text = new QLineEdit();
            text->setFont(gostFont);
            text->setAlignment(Qt::AlignCenter);
            text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            QLabel *lab = new QLabel(tr("&Content:"));
            QString attr = "text";
            if (aBlock->type() == "process")
            {
                dlg.setWindowTitle(tr("Process"));
            }
            else if (aBlock->type() == "if")
            {
                dlg.setWindowTitle(tr("Branching"));
                lab->setText(tr("&Condition:"));
                attr = "cond";
            }
            else if (aBlock->type() == "pre")
            {
                dlg.setWindowTitle(tr("WHILE loop"));
                lab->setText(tr("&Condition:"));
                attr = "cond";
            }
            else if (aBlock->type() == "post")
            {
                dlg.setWindowTitle(tr("Post-condition loop"));
                lab->setText(tr("&Condition:"));
                attr = "cond";
            }
            text->setText(aBlock->attributes.value(attr, ""));
            lab->setBuddy(text);
            QHBoxLayout *box = new QHBoxLayout;
            box->addWidget(lab);
            box->addWidget(text, 1);
            mainLayout->addLayout(box);
            mainLayout->addLayout(buttonLayout);

            if (dlg.exec() == QDialog::Accepted)
            {
                if(aBlock->flowChart())
                {
                    aBlock->flowChart()->makeUndo();
                    aBlock->attributes[attr] = text->text();
                    aBlock->flowChart()->realignObjects();
                    aBlock->flowChart()->update();
                    aBlock->flowChart()->makeChanged();
                }
            }
        }
        else if(aBlock->type() == "for")
        {
            dlg.setWindowTitle(tr("FOR loop"));
            dlg.setMinimumHeight(180);
            QGridLayout *gl = new QGridLayout();
            QLineEdit *teVar = new QLineEdit();
            QLineEdit *teFrom = new QLineEdit();
            QLineEdit *teTo = new QLineEdit();
            teVar->setFont(gostFont);
            teFrom->setFont(gostFont);
            teTo->setFont(gostFont);
            teVar->setAlignment(Qt::AlignCenter);
            teFrom->setAlignment(Qt::AlignCenter);
            teTo->setAlignment(Qt::AlignCenter);
            teVar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            teFrom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            teTo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            QLabel *labVar = new QLabel(tr("&Variable:"));
            QLabel *labFrom = new QLabel(tr("&Start value:"));
            QLabel *labTo = new QLabel(tr("&End value:"));
            gl->addWidget(labVar, 0,0);
            gl->addWidget(teVar, 0,1);
            gl->addWidget(labFrom, 1,0);
            gl->addWidget(teFrom, 1,1);
            gl->addWidget(labTo, 2,0);
            gl->addWidget(teTo, 2,1);
            labFrom->setBuddy(teFrom);
            labTo->setBuddy(teTo);
            labVar->setBuddy(teVar);
            gl->setColumnStretch(1, 1);

            mainLayout->addLayout(gl, 1);
            mainLayout->addLayout(buttonLayout);
            teVar->setText(aBlock->attributes.value("var", ""));
            teFrom->setText(aBlock->attributes.value("from", ""));
            teTo->setText(aBlock->attributes.value("to", ""));
            if (dlg.exec() == QDialog::Accepted)
            {
                if(aBlock->flowChart())
                {
                    aBlock->flowChart()->makeUndo();
                    aBlock->attributes["var"] = teVar->text();
                    aBlock->attributes["from"] = teFrom->text();
                    aBlock->attributes["to"] = teTo->text();
                    aBlock->flowChart()->realignObjects();
                    aBlock->flowChart()->update();
                    aBlock->flowChart()->makeChanged();
                }
            }
        }
        else if(aBlock->type() == "io" || aBlock->type() == "ou")
        {
            if (aBlock->type() == "io")
            {
                    dlg.setWindowTitle(tr("Input"));
                }
            else if (aBlock->type() == "ou")
            {
                    dlg.setWindowTitle(tr("Output"));
                }
            dlg.setMinimumHeight(300);
            QVBoxLayout *vl = new QVBoxLayout();
            QTextEdit *te = new QTextEdit();
            te->setAcceptRichText(false);
            te->setFont(gostFont);
            te->setMinimumHeight(160);
            te->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            QLabel *lab = new QLabel(tr("Specify the names of the variables (each per line):"));
            vl->addWidget(lab);
            vl->addWidget(te, 1);  // stretch factor = 1 для расширения
            mainLayout->addLayout(vl, 1);  // stretch factor для всего layout
            mainLayout->addLayout(buttonLayout);
            te->setText(aBlock->attributes.value("vars").split(",").join("\n"));


            if (dlg.exec() == QDialog::Accepted)
            {


                if(aBlock->flowChart())
                {
                    aBlock->flowChart()->makeUndo();
                    aBlock->attributes["vars"] = te->toPlainText().split("\n", Qt::SkipEmptyParts).join(",");
                    aBlock->flowChart()->realignObjects();
                    aBlock->flowChart()->update();
                    aBlock->flowChart()->makeChanged();
                }
            }
        }

        else if(aBlock->type() == "assign")
        {
            dlg.setWindowTitle(tr("Assign"));
            dlg.setMinimumHeight(140);
            QGridLayout *gl = new QGridLayout();
            QLineEdit *leSrc = new QLineEdit();
            QLineEdit *leDest = new QLineEdit();
            leSrc->setFont(gostFont);
            leDest->setFont(gostFont);
            leSrc->setAlignment(Qt::AlignCenter);
            leDest->setAlignment(Qt::AlignCenter);
            leSrc->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            leDest->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            QLabel *labSrc = new QLabel(tr("&Source:"));
            QLabel *labDest = new QLabel(tr("&Destination:"));
            gl->addWidget(labDest, 0,0);
            gl->addWidget(leDest, 0,1);
            gl->addWidget(labSrc, 1,0);
            gl->addWidget(leSrc, 1,1);
            labDest->setBuddy(leDest);
            labSrc->setBuddy(leSrc);
            gl->setColumnStretch(1, 1);

            mainLayout->addLayout(gl, 1);
            mainLayout->addLayout(buttonLayout);
            leSrc->setText(aBlock->attributes.value("src", ""));
            leDest->setText(aBlock->attributes.value("dest", ""));

            if (dlg.exec() == QDialog::Accepted)
            {

                if(aBlock->flowChart())
                {
                    aBlock->flowChart()->makeUndo();
                    aBlock->attributes["dest"] = leDest->text();
                    aBlock->attributes["src"] = leSrc->text();
                    aBlock->flowChart()->realignObjects();
                    aBlock->flowChart()->update();
                    aBlock->flowChart()->makeChanged();
                }
            }
        }

    }
}

void MainWindow::slotToolAssign()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><assign dest=\"x\" src=\"0\"/></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }
}

void MainWindow::slotToolArrow()
{
    if(document())
    {
        document()->setStatus(QFlowChart::Selectable);
        document()->update();
    }
}
void MainWindow::slotToolProcess()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><process text=\"func()\"/></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }
}

void MainWindow::slotToolIf()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><if cond=\"x &gt; 0\"><branch /><branch /></if></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }
}

void MainWindow::slotToolFor()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><for var=\"i\" from=\"0\" to=\"n - 1\"><branch /></for></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }
}

void MainWindow::slotToolWhilePre()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><pre cond=\"x &lt; n\"><branch /></pre></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }
}

void MainWindow::slotToolWhilePost()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><post cond=\"x &lt; n\"><branch /></post></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }

}

void MainWindow::slotToolIo()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><io vars=\"x,y\"/></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }

}




void MainWindow::slotToolOu()
{
    if(document())
    {
        document()->setBuffer("<algorithm><branch><ou vars=\"x,y\"/></branch></algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }

}

void MainWindow::slotToolCase()
{
    //  if(document())
    //  {
    //    document()->setBuffer("<algorithm><branch><case><branch /><branch /><branch /></case></branch></algorithm>");
    //    if(!document()->buffer().isEmpty())
    //    {
    //      document()->setStatus(QFlowChart::Insertion);
    //      document()->setMultiInsert(false);
    //    }
    //  }

}

void MainWindow::slotToolForCStyle()
{
    if(document())
    {
        document()->setBuffer("<algorithm> <branch> <assign dest=\"i\" src=\"0\"  /> <pre cond=\"i &lt; n\"> <branch> <assign dest=\"i\" src=\"i + 1\" /> </branch> </pre> </branch> </algorithm>");
        if(!document()->buffer().isEmpty())
        {
            document()->setStatus(QFlowChart::Insertion);
            document()->setMultiInsert(false);
        }
    }

}

