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

#include "zvflowchart.h"
#include <QApplication>

namespace {
void initBlockDefaults(QBlock *block)
{
  block->parent = 0;
  block->isBranch = false;
  block->attributes.clear();
  block->items.clear();
  block->setFlowChart(0);
  block->x = 0;
  block->y = 0;
  block->width = 0;
  block->height = 0;
  block->topMargin = 0;
  block->bottomMargin = 0;
  block->leftMargin = 0;
  block->rightMargin = 0;
}
}

QFlowChart::QFlowChart(QWidget *pObj /* = 0 */) : QWidget(pObj), fZoom(1)
{
  fBuffer = QString();
  fTargetPoint = QInsertionPoint();
  fStatus = Display;
  fRoot = new QBlock();
  root()->setFlowChart(this);
  clear();
  setZoom(1);
}

QFlowChart::~QFlowChart()
{
  clear();
}

void QFlowChart::makeUndo()
{
  QString state = toString();
  undoStack.push(state);
  redoStack.clear();
  emit modified();
}
bool QFlowChart::canUndo() const
{
  return !undoStack.isEmpty();
}

bool QFlowChart::canRedo() const
{
  return !redoStack.isEmpty();
}

bool QFlowChart::canPaste() const
{
  QDomDocument doc;
  QClipboard *clp = QApplication::clipboard();
  QString aBuffer = clp->text();
  return doc.setContent(aBuffer, false);
}

void QFlowChart::makeChanged()
{
  emit changed();
}

void QFlowChart::undo()
{
  if (!undoStack.isEmpty())
  {
    QString state = toString();
    redoStack.push(state);
    state = undoStack.pop();
    fromString(state);
    deselectAll();
    emit changed();
  }
}


void QFlowChart::redo()
{
  if (!redoStack.isEmpty())
  {
    QString state = toString();
    undoStack.push(state);
    state = redoStack.pop();
    fromString(state);
    deselectAll();
    emit changed();
  }
}

QDomDocument QFlowChart::document() const
{
  QDomDocument doc("AFC");
  QDomElement r = root()->xmlNode(doc);
  doc.appendChild(r);
  return doc;
}

QBlock * QFlowChart::root() const
{
  return fRoot;
}

QBlock * QFlowChart::activeBlock() const
{
  return fActiveBlock;
}

void QFlowChart::setBuffer(const QString & aBuffer)
{
  QDomDocument doc;
  if(doc.setContent(aBuffer, false))
  {
    fBuffer = aBuffer;
  }
  else
  {
    fBuffer = QString();
  }
}

void QFlowChart::makeBackwardCompatibility() {
    if(root()) {
        root()->makeBackwardCompatibility();
    }
}

QString QFlowChart::toString()
{
  return document().toString(2);
}

void QFlowChart::fromString(const QString & str)
{
  QDomDocument doc;
  if(doc.setContent(str, false))
  {
    root()->setXmlNode(doc.firstChildElement("algorithm"));
    realignObjects();
    emit changed();
  }
}


/******************************** QBlock ***********************************/


QBlock::QBlock()
{
  initBlockDefaults(this);
}

QBlock::QBlock(const QString &aType)
{
  initBlockDefaults(this);
  setType(aType);
}

QBlock::~QBlock()
{
  if(parent != 0)
  {
    parent->remove(this);
    clear();
  }
}

void QBlock::makeBackwardCompatibility() {

    // it supports obsoletted attributes t1, t2, ..., t8
    // and converts to attribute vars with comma delemited values
    // versions before 0.9.7
    if(type() == "io" || type() == "ou") {
        QStringList sl;
        for(int i = 1; i <= 8; ++i) {
            QString attr = QString("t%1").arg(i);
            if(attributes.value(attr, "") != "")
                sl << attributes.value(attr, "");
            attributes.remove(attr);
        }
        if(!sl.empty()) {
            QString vars = sl.join(",");
            attributes.insert("vars", vars);
        }
    }

    if(type() == "algorithm") {
        attributes.insert("version", AFC_VERSION);
    }

    for(int i = 0; i < items.size(); ++i) {
        items.at(i)->makeBackwardCompatibility();
    }
}

void QBlock::clear()
{
  while(!items.isEmpty())
  {
    deleteObject(0);
  }
  QString currentType = type();
  attributes.clear();
  setType(currentType);
  items.clear();
}

QDomElement QBlock::xmlNode(QDomDocument & doc) const
{
  QDomElement self = doc.createElement(type());
  QList<QString> sl = attributes.uniqueKeys();
  for (int i = 0; i < sl.size(); ++i)
  {
    if (sl.at(i) != "type")
    {
      self.setAttribute(sl.at(i), attributes.value(sl.at(i), QString()));
    }
  }


  for (int i = 0; i < items.size(); ++i)
  {
    QDomElement child = item(i)->xmlNode(doc);
    self.appendChild(child);
  }
  return self;
}

void QBlock::setXmlNode(const QDomElement & node)
{
  clear();
  setType(node.nodeName());
  QDomNamedNodeMap attrs = node.attributes();
  for (int i = 0; i < attrs.size(); ++i)
  {
    QDomAttr da = attrs.item(i).toAttr();
    if(da.name() != "type")
    {
      attributes.insert(da.name(), da.value());
    }
  }
  if (type() == "branch") isBranch = true;
  else isBranch = false;
  QDomNodeList children = node.childNodes();
  for(int i = 0; i < children.size(); ++i)
  {
    if (children.at(i).isElement())
    {
      QDomElement child = children.at(i).toElement();
      QBlock *block = new QBlock();
      block->setFlowChart(flowChart());
      block->setXmlNode(child);
      append(block);
    }
  }
}

void QBlock::insertXmlTree(int aIndex, const QDomElement & algorithm)
{
  if (isBranch)
  {
    QDomElement branch = algorithm.firstChildElement("branch");
    if(!branch.isNull())
    {
      QDomNodeList children = branch.childNodes();
      int ind = aIndex;
      for(int i = 0; i < children.size(); ++i)
      {
        if (children.at(i).isElement())
        {
          QDomElement child = children.at(i).toElement();
          QBlock *block = new QBlock();
          block->setFlowChart(flowChart());
          block->setXmlNode(child);
          insert(ind, block);
          ind++;
        }
      }
    }
  }
}
