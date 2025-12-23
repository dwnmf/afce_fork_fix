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

void QFlowChart::clear()
{
    deselectAll();
    root()->clear();
    root()->attributes.clear();
    root()->setType("algorithm");
    QBlock *branch = new QBlock();
    branch->setType("branch");
    branch->isBranch = true;
    root()->append(branch);
    branch->setFlowChart(root()->flowChart());

//    fDocument->clear();
//    QDomProcessingInstruction xml = fDocument->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\" stand-alone=\"yes\"");
//    fDocument->appendChild(xml);

//    QDomElement fRoot = fDocument->createElement("algorithm");
//    fDocument->appendChild(fRoot);
//    QDomElement branch = fDocument->createElement("branch");
//    fRoot.appendChild(branch);
    emit changed();
}

void QFlowChart::selectAll()
{
  fActiveBlock = root();
  emit changed();
  update();
}

void QFlowChart::deselectAll()
{
    fActiveBlock = 0;
    emit changed();
    update();
}

void QFlowChart::setStatus(int aStatus)
{
  fStatus = aStatus;
  if (status() == Insertion)
  {
    setMouseTracking(true);
    regeneratePoints();
    update();
  }
  else
  {
    fTargetPoint = QInsertionPoint();
    setMouseTracking(false);
  }
  emit statusChanged();
  emit changed();
}

void QFlowChart::deleteActiveBlock()
{
  if (activeBlock())
  {
    makeUndo();
    QBlock *tmp = activeBlock();
    fActiveBlock = 0;
    deleteBlock(tmp);
    emit changed();
  }
}

void QFlowChart::mousePressEvent(QMouseEvent *pEvent)
{
  if (status() == Selectable)
  {
    QBlock *block = root()->blockAt(pEvent->x(), pEvent->y());
    if (block)
    {
      if (block->isActive() && ((pEvent->modifiers() & Qt::ControlModifier) != 0))
      {
        if(activeBlock()->parent)
        {
          fActiveBlock = activeBlock()->parent;
        }
        else
        {
          fActiveBlock = block;
        }
      }
      else
      {
        if(block->parent !=0 && block->isBranch)
        {
          fActiveBlock = block->parent;
        }
        else
          fActiveBlock = block;

      }
      emit changed();
      update();
    }
  }
  else if(status() == Insertion)
  {
    QPoint mp = pEvent->pos();
    QInsertionPoint ip = getNearistPoint(mp.x(), mp.y());
    fTargetPoint = ip;
    if(!ip.isNull() && !buffer().isEmpty())
    {
      QDomDocument doc;
      if(doc.setContent(buffer(), false))
      {
        QDomElement algorithm = doc.firstChildElement("algorithm");
        QBlock *branch = ip.branch();
        if(branch)
        {
          makeUndo();
          branch->insertXmlTree(ip.index(), algorithm);
          realignObjects();
          regeneratePoints();
          fActiveBlock = 0;
          emit changed();
        }
      }
    }
    if (!multiInsert()) setStatus(Selectable);
  }
}

void QFlowChart::mouseDoubleClickEvent(QMouseEvent * event)
{
  if(status() == Selectable && event->modifiers() == Qt::NoModifier)
  {
    QBlock *block = root()->blockAt(event->x(), event->y());
    if (block)
    {
      emit changed();
      emit editBlock(block);
    }
  }
}

void QFlowChart::mouseMoveEvent(QMouseEvent *pEvent)
{
  if(status() == Insertion)
  {
    QPoint mp = pEvent->pos();
    QInsertionPoint ip = getNearistPoint(mp.x(), mp.y());
    fTargetPoint = ip;
    repaint();
  }
}

void QFlowChart::deleteBlock(QBlock *aBlock)
{
  if (aBlock == root())
  {
    for(int i = 0; i < aBlock->items.size(); ++i)
    {
      aBlock->item(i)->clear();
    }
    realignObjects();
  }
  else if (aBlock->isBranch)
  {
    aBlock->clear();
    realignObjects();
  }
  else
  {
    delete aBlock;
    realignObjects();
  }
  emit changed();
}


/******************************** QBlock ***********************************/


QBlock * QBlock::root()
{
  if (parent == 0)
  {
    return this;
  }
  else
  {
    return parent->root();
  }
}

int QBlock::index()
{

  if(parent == 0) return -1;
  else return parent->items.indexOf(this);
}

void QBlock::insert(int newIndex, QBlock *aBlock)
{
  if (aBlock->parent != 0)
  {
    aBlock->parent->remove(aBlock);
  }
  if (newIndex < 0 || newIndex >= items.size())
    items.append(aBlock);
  else
    items.insert(newIndex, aBlock);
  aBlock->parent = this;
  aBlock->setFlowChart(flowChart());
}

void QBlock::remove(QBlock *aBlock)
{
  items.removeAll(aBlock);
  aBlock->parent = 0;
  aBlock->setFlowChart(0);
}

void QBlock::append(QBlock *aBlock)
{
  insert(-1, aBlock);
}

void QBlock::deleteObject(int aIndex)
{
  QBlock *tmp = item(aIndex);
  if(tmp)
  {
    delete tmp;
  }
}


void QBlock::setItem(int aIndex, QBlock *aBlock)
{
  if(items.size() > aIndex && aIndex >= 0)
  {
    if (aBlock->parent != 0)
    {
      aBlock->parent->remove(aBlock);
    }
    QBlock *old = item(aIndex);
    old->parent = 0;
    items.replace(aIndex, aBlock);
  }
}

void QBlock::setFlowChart(QFlowChart * aFlowChart)
{
  fFlowChart = aFlowChart;
}

QBlock * QBlock::blockAt(int px, int py)
{
  QRectF rect(x, y, width, height);
  if (!rect.contains(px, py)) return 0;
  else
  {
    for (int i = 0; i < items.size(); ++i)
    {
      QBlock *tmp = item(i)->blockAt(px, py);
      if (tmp) return tmp;
    }
    return this;
  }
}

bool QBlock::isActive() const
{
  if(flowChart())
  {
    if (flowChart()->activeBlock() == this) return true;
    else if (parent)
    {
      return parent->isActive();
    }
    else
    {
      return false;
    }

  }
  else
    return false;
}
