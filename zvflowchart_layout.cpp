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

namespace {
QFont blockFont(double zoom)
{
  QFont font("Tahoma");
  font.setPixelSize(13 * zoom);
  return font;
}

double textWidthWithPadding(const QString &text, double zoom, double padding)
{
  QFontMetrics fm(blockFont(zoom));
  return fm.horizontalAdvance(text) + padding;
}

QString normalizedVars(const QString &vars)
{
  QStringList list = vars.split(",");
  return list.join(", ");
}
}

void QFlowChart::setZoom(const double aZoom)
{
  fZoom = aZoom;
  realignObjects();
  if (status() == Insertion)
  {
    fTargetPoint = QInsertionPoint();
    regeneratePoints();
  }
  emit zoomChanged(aZoom);
}

void QFlowChart::realignObjects()
{
  if(root())
  {
    makeBackwardCompatibility();
    root()->adjustSize(zoom());
    root()->adjustPosition(0,0);
    resize(root()->width, root()->height);
    emit changed();
    update();
  }
}

QSize QFlowChart::sizeHint() const
{
  if (root())
  {
    return QSize(root()->width, root()->height);
  }
  else return QSize();

}

QInsertionPoint QFlowChart::getNearistPoint(int x, int y) const
{
  QInsertionPoint result;
  if (insertionPoints.size() > 0)
  {
    result = insertionPoints.at(0);
    double len = calcLength(result.point(), QPointF(x, y));
    for (int i = 0; i < insertionPoints.size(); ++i)
    {
      QInsertionPoint ip = insertionPoints.at(i);
      double tmp = calcLength(ip.point(), QPointF(x, y));
      if (tmp < len)
      {
        result = ip;
        len = tmp;
      }
    }
  }
  return result;
}

void QFlowChart::regeneratePoints()
{
  insertionPoints.clear();
  if(root())
  {
    generatePoints(root());
  }
}

void QFlowChart::generatePoints(QBlock *aBlock)
{
  if(aBlock->isBranch)
  {
    double x = aBlock->x + aBlock->width / 2.0;
    for (int i = 0; i < aBlock->items.size(); ++i)
    {
      double y = aBlock->item(i)->y;
      QInsertionPoint p;
      p.setBranch(aBlock);
      p.setPoint(QPointF(x, y));
      p.setIndex(i);
      insertionPoints.append(p);
      generatePoints(aBlock->item(i));
    }
    QInsertionPoint p;
    p.setBranch(aBlock);
    if(aBlock->items.size() == 0)
      p.setPoint(QPointF(x, aBlock->y + aBlock->height / 2));
    else
      p.setPoint(QPointF(x, aBlock->y + aBlock->height));
    p.setIndex(aBlock->items.size());
    insertionPoints.append(p);
  }
  else
  {
    for (int i = 0; i < aBlock->items.size(); ++i)
    {
      generatePoints(aBlock->item(i));
    }
  }
}

double QFlowChart::calcLength(const QPointF & p1, const QPointF & p2)
{
  return (p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y());
}

void QFlowChart::setChartStyle(const QFlowChartStyle & aStyle)
{
  fStyle = aStyle;
  emit changed();
  update();
}

double QFlowChart::zoom() const
{
  return fZoom;
}

void QFlowChart::drawBottomArrow(QPainter *canvas, const QPointF & aPoint, const QSizeF & aSize)
{
  QVector<QPointF> arrow;
  arrow << QPointF(aPoint.x() - aSize.width()/2.0, aPoint.y()-aSize.height()) <<
      aPoint << aPoint << QPointF(aPoint.x() + aSize.width()/2.0, aPoint.y()-aSize.height());
  canvas->drawLines(arrow);
}

void QFlowChart::drawRightArrow(QPainter *canvas, const QPointF & aPoint, const QSizeF & aSize)
{
  QVector<QPointF> arrow;
  arrow << QPointF(aPoint.x() - aSize.width(), aPoint.y()-aSize.height()/2);
  arrow << aPoint;
  arrow << aPoint;
  arrow << QPointF(aPoint.x() - aSize.width(), aPoint.y()+aSize.height()/2);
  canvas->drawLines(arrow);
}


/******************************** QBlock ***********************************/


void QBlock::adjustSize(const double aZoom)
{
  double clientWidth = 0, clientHeight = 0;
  if (isBranch)
  {
    for (int i = 0; i < items.size(); ++i)
    {
      item(i)->adjustSize(aZoom);
      if (clientWidth < item(i)->width) clientWidth = item(i)->width;
      clientHeight += item(i)->height;
    }
    double minWidth = 180 * aZoom;
    double minHeight = 16 * aZoom;
    if (clientHeight < minHeight) clientHeight = minHeight;
    if (clientWidth < minWidth) clientWidth = minWidth;
    height = clientHeight;
    width = clientWidth;

  }
  else
  {
    for (int i = 0; i < items.size(); ++i)
    {

      item(i)->adjustSize(aZoom);
      if (clientHeight < item(i)->height) clientHeight = item(i)->height;
      clientWidth += item(i)->width;
    }
    /* поля по умолчанию */
    topMargin = 60 * aZoom;
    bottomMargin = 60 * aZoom;
    leftMargin = 10 * aZoom;
    rightMargin = 10 * aZoom;
    if (type() == "algorithm")
    {
      topMargin = 40 * aZoom;
      bottomMargin = 50 * aZoom;
    }
    else if (type() == "process")
    {
      topMargin = 16 * aZoom;
      bottomMargin = 10 * aZoom;
      double textWidth = textWidthWithPadding(attributes.value("text", ""), aZoom, 16 * aZoom);
      double minWidth = 120 * aZoom;
      clientWidth = qMax(minWidth, textWidth);
      clientHeight = 60 * aZoom;
    }
    else if (type() == "assign")
    {
      topMargin = 16 * aZoom;
      bottomMargin = 10 * aZoom;
      QString text = QString("%1 := %2").arg(attributes.value("dest", ""), attributes.value("src", ""));
      double textWidth = textWidthWithPadding(text, aZoom, 16 * aZoom);
      double minWidth = 120 * aZoom;
      clientWidth = qMax(minWidth, textWidth);
      clientHeight = 60 * aZoom;
    }
    else if (type() == "io")
    {
      topMargin = 16 * aZoom;
      bottomMargin = 10 * aZoom;
      leftMargin = 20 * aZoom;
      rightMargin = 20 * aZoom;
      QString text = normalizedVars(attributes.value("vars", ""));
      double textWidth = textWidthWithPadding(text, aZoom, 20 * aZoom);
      double minWidth = 120 * aZoom;
      clientWidth = qMax(minWidth, textWidth);
      clientHeight = 60 * aZoom;
    }
    else if (type() == "ou")
    {
      topMargin = 16 * aZoom;
      bottomMargin = 10 * aZoom;
      leftMargin = 20 * aZoom;
      rightMargin = 20 * aZoom;
      QString text = normalizedVars(attributes.value("vars", ""));
      double textWidth = textWidthWithPadding(text, aZoom, 20 * aZoom);
      double minWidth = 120 * aZoom;
      clientWidth = qMax(minWidth, textWidth);
      clientHeight = 60 * aZoom;
    }
    else if (type() == "if")
    {
      topMargin = 92 * aZoom;
      bottomMargin = 16 * aZoom;
    }
    else if (type() == "pre")
    {
      topMargin = 108 * aZoom;
      bottomMargin = 32 * aZoom;
    }
    else if (type() == "post")
    {
      topMargin = 16 * aZoom;
      bottomMargin = 96 * aZoom;
    }
    else if (type() == "for")
    {
      topMargin = 108 * aZoom;
      bottomMargin = 32 * aZoom;
    }

    width = leftMargin + clientWidth + rightMargin;
    height = topMargin + clientHeight + bottomMargin;
  }
}

void QBlock::adjustPosition(const double ox, const double oy)
{
  x = ox;
  y = oy;
  if (isBranch)
  {
    double cy = y;
    for (int i = 0; i < items.size(); ++i)
    {
      item(i)->adjustPosition(ox + (width - item(i)->width) / 2, cy);
      cy += item(i)->height;
    }
  }
  else
  {
    double cx = x + leftMargin;
    for (int i = 0; i < items.size(); ++i)
    {
      item(i)->adjustPosition(cx, y + topMargin);
      cx += item(i)->width;
    }
  }
}

double QBlock::zoom() const
{
  if (flowChart())
  {
    return flowChart()->zoom();
  }
  else
  {
    return 1;
  }
}
