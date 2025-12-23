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

void QFlowChart::paintEvent(QPaintEvent *pEvent)
{
    QPainter canvas(this);
    pEvent->accept();
    canvas.setClipRect(pEvent->rect());
    canvas.setRenderHint(QPainter::Antialiasing, true);
    paintTo(&canvas);
}

void QFlowChart::paintTo(QPainter *canvas)
{
    if (root())
    {
      root()->paint(canvas);
      if (status() == Insertion)
      {
        QFlowChartStyle st = chartStyle();
        for (int i = 0; i < insertionPoints.size(); ++i)
        {
          QInsertionPoint ip = insertionPoints.at(i);
          QPointF p = ip.point();
          canvas->setPen(QPen(st.normalForeground(), 2 * zoom()));
          canvas->setBrush(st.normalForeground());
          canvas->drawEllipse(p, 3 * zoom(), 3 * zoom());
        }
        if (!targetPoint().isNull())
        {
          QPointF p = targetPoint().point();
          canvas->setPen(QPen(st.selectedBackground(), 2 * zoom()));
          canvas->setBrush(st.selectedBackground());
          canvas->drawEllipse(p, 7 * zoom(), 7 * zoom());
        }
      }
    }
}


/******************************** QBlock ***********************************/


void QBlock::drawCaption(QPainter *canvas, const QRectF & rect, const double zoomFactor, const QString & text)
{
  QFont font("Sans Serif");
  font.setPixelSize(13 * zoomFactor);
  font = QFont(font, canvas->device());
  QFontMetricsF fontMetrics(font);
  QRectF textRect = fontMetrics.boundingRect(text);
  double tx = rect.x() + rect.width() / 2 - textRect.width() / 2;
  double ty = rect.y() + rect.height() / 2 + fontMetrics.ascent() / 2;
  canvas->setFont(font);
//  canvas->drawRect(tx, ty - textRect.height(), textRect.width(), textRect.height());
  canvas->drawText(QPointF(tx, ty), text);

}

void QBlock::paint(QPainter *canvas, bool fontSizeInPoints) const
{
  if (flowChart())
  {
    QFlowChartStyle st = flowChart()->chartStyle();
    double hcenter = x + width / 2;
    /* в соответствии с ГОСТ 19.003-80 */
    double a = 60 * zoom();
    double b = 2 * a;
    double bottom = y + height;
    double lw = st.lineWidth() * zoom();

//    QFont font = flowChart()->font();
//    font.setPixelSize(13 * zoom());
//    flowChart()->setFont(font);

    QFont font("Tahoma");
    font.setWeight(0);
    if (fontSizeInPoints)
      font.setPointSizeF(10 * zoom());
    else
      font.setPixelSize(13 * zoom());
    font = QFont(font, canvas->device());
    canvas->setFont(font);

    if(flowChart()->status() == QFlowChart::Selectable && isActive())
    {
      canvas->setPen(QPen(st.selectedForeground(), lw));
      canvas->setBrush(QBrush(st.selectedBackground()));
    }
    else
    {
      canvas->setPen(QPen(st.normalForeground(), lw));
      canvas->setBrush(QBrush(st.normalBackground()));
    }

    QPen pen = canvas->pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    canvas->setPen(pen);
    canvas->fillRect(QRectF(x, y, width, height), canvas->brush());

    if (isBranch)
    {
      /* отрисовка ветви */
      QLineF line(hcenter, y-0.5, hcenter, y + height+0.5);
      canvas->drawLine(line);
    }
    else
    {
      // Modern shadow offset for box-shadow effect
      double shadowOffset = 4 * zoom();
      QColor shadowColor(0, 0, 0, 40); // Subtle semi-transparent shadow
      
      if (type() == "algorithm")
      {
        /* алгоритм */
        QBlock *body = item(0);
        Q_ASSERT_X(body != 0, "QBlock::paint()" ,"item(0) == 0. i.e. body of algorithm is nul.");
        
        // Draw shadow for BEGIN block
        QRectF shadowOval(hcenter - b/2 + shadowOffset, lw + shadowOffset, b, a/2);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawRoundedRect(shadowOval, a/4, a/4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QRectF oval(hcenter - b/2, lw, b, a/2);
        canvas->drawRoundedRect(oval, a/4, a/4);
        canvas->drawText(oval, Qt::TextSingleLine | Qt::AlignHCenter | Qt::AlignVCenter, tr("BEGIN"));
//        drawCaption(canvas, oval, zoom(), tr("BEGIN"));
        canvas->drawLine(QLineF(hcenter, y + a/2+lw, hcenter, body->y+0.5));
        canvas->drawLine(QLineF(hcenter, body->y + body->height-0.5, hcenter, bottom - a/2-lw));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, bottom - a/2 - lw),
                                    QSize(6 * zoom(), 12 * zoom()));

        // Draw shadow for END block
        QRectF shadowOvalEnd(hcenter - b/2 + shadowOffset, bottom - a/2 - lw + shadowOffset, b, a/2);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawRoundedRect(shadowOvalEnd, a/4, a/4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        oval = QRectF(hcenter - b/2, bottom - a/2 - lw, b, a/2);
        canvas->drawRoundedRect(oval, a/4, a/4);
        canvas->drawText(oval, Qt::TextSingleLine | Qt::AlignHCenter | Qt::AlignVCenter, tr("END"));

      }
      else if(type() == "process")
      {
        /* процесс */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 16 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 16 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        // Используем динамическую ширину блока
        double blockWidth = width - leftMargin - rightMargin;
        
        // Draw shadow
        QRectF shadowRect(hcenter - blockWidth/2 + shadowOffset, y + 16 * zoom() + shadowOffset, blockWidth, a);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawRect(shadowRect);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QRectF rect(hcenter - blockWidth/2, y + 16 * zoom(), blockWidth, a);
        QRectF textRect(hcenter - blockWidth/2 + 4 * zoom(), y + 20 * zoom(), blockWidth - 8 * zoom(), a - 8 * zoom());
        canvas->drawRect(rect);
        canvas->drawText(textRect, Qt::AlignCenter | Qt::TextWrapAnywhere, attributes.value("text", ""));
        canvas->drawLine(QLineF(hcenter, y + 16 * zoom()+a, hcenter, bottom+0.5));
      }
      else if(type() == "assign")
      {
        /* присваивание */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 16 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 16 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        // Используем динамическую ширину блока
        double blockWidth = width - leftMargin - rightMargin;
        
        // Draw shadow
        QRectF shadowRect(hcenter - blockWidth/2 + shadowOffset, y + 16 * zoom() + shadowOffset, blockWidth, a);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawRect(shadowRect);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QRectF rect(hcenter - blockWidth/2, y + 16 * zoom(), blockWidth, a);
        QRectF textRect(hcenter - blockWidth/2+4, y + 16 * zoom()+4, blockWidth-8, a-8);
        canvas->drawRect(rect);
        canvas->drawText(textRect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString("%1 := %2").arg(attributes.value("dest", ""), attributes.value("src", "")));
        canvas->drawLine(QLineF(hcenter, y + 16 * zoom()+a, hcenter, bottom+0.5));
      }
      else if(type() == "io")
      {
        /* ввод/вывод */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 16 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 16 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        // Используем динамическую ширину блока вместо фиксированной b
        double blockWidth = width - leftMargin - rightMargin;
        
        // Draw shadow parallelogram
        QPointF shadowPar[4];
        shadowPar[0] = QPointF(hcenter - blockWidth/2 + a/4 + shadowOffset, y + 16 * zoom() + shadowOffset);
        shadowPar[1] = QPointF(hcenter + blockWidth/2 + a/4 + shadowOffset, y + 16 * zoom() + shadowOffset);
        shadowPar[2] = QPointF(hcenter + blockWidth/2 - a/4 + shadowOffset, y + 16 * zoom() + a + shadowOffset);
        shadowPar[3] = QPointF(hcenter - blockWidth/2 - a/4 + shadowOffset, y + 16 * zoom() + a + shadowOffset);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawPolygon(shadowPar, 4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QPointF par[4];
        par[0] = QPointF(hcenter - blockWidth/2 + a/4, y + 16 * zoom());
        par[1] = QPointF(hcenter + blockWidth/2 + a/4, y + 16 * zoom());
        par[2] = QPointF(hcenter + blockWidth/2 - a/4, y + 16 * zoom() + a);
        par[3] = QPointF(hcenter - blockWidth/2 - a/4, y + 16 * zoom() + a);
        canvas->drawPolygon(par, 4);
        QRectF rect(hcenter - blockWidth/2, y + 16 * zoom(), blockWidth, a);
        QStringList ls = attributes["vars"].split(",");

        QString text = ls.join(", ");
        canvas->drawText(rect, Qt::TextSingleLine | Qt::AlignHCenter | Qt::AlignVCenter, text);
        canvas->drawLine(QLineF(hcenter, y + 16 * zoom()+a, hcenter, bottom+0.5));
      }
      else if(type() == "ou")
      {
        /* ввод/вывод */
        canvas->drawLine(QLineF(hcenter, y, hcenter, y + 16 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 16 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        // Используем динамическую ширину блока вместо фиксированной b
        double blockWidth = width - leftMargin - rightMargin;
        
        // Draw shadow parallelogram
        QPointF shadowPar[4];
        shadowPar[0] = QPointF(hcenter - blockWidth/2 + a/4 + shadowOffset, y + 16 * zoom() + shadowOffset);
        shadowPar[1] = QPointF(hcenter + blockWidth/2 + a/4 + shadowOffset, y + 16 * zoom() + shadowOffset);
        shadowPar[2] = QPointF(hcenter + blockWidth/2 - a/4 + shadowOffset, y + 16 * zoom() + a + shadowOffset);
        shadowPar[3] = QPointF(hcenter - blockWidth/2 - a/4 + shadowOffset, y + 16 * zoom() + a + shadowOffset);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawPolygon(shadowPar, 4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QPointF par[4];
        par[0] = QPointF(hcenter - blockWidth/2 + a/4, y + 16 * zoom());
        par[1] = QPointF(hcenter + blockWidth/2 + a/4, y + 16 * zoom());
        par[2] = QPointF(hcenter + blockWidth/2 - a/4, y + 16 * zoom() + a);
        par[3] = QPointF(hcenter - blockWidth/2 - a/4, y + 16 * zoom() + a);
        canvas->drawPolygon(par, 4);
        QRectF textRect(hcenter - blockWidth/2 + a/4 +4, y + 16 * zoom()+4, blockWidth-a/2 - 8, a - 8);
        QStringList ls = attributes["vars"].split(",");
        QString text = ls.join(", ");
        canvas->drawText(textRect, Qt::TextSingleLine | Qt::AlignHCenter | Qt::AlignVCenter, text);
        canvas->drawLine(QLineF(hcenter, y + 16 * zoom()+a, hcenter, bottom+0.5));
      }
      else if(type() == "if")
      {
        /* ветвление */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 16 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 16 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        
        // Draw shadow diamond
        QPointF shadowPar[4];
        shadowPar[0] = QPointF(hcenter - b/2 + shadowOffset, y + 16 * zoom() + a/2 + shadowOffset);
        shadowPar[1] = QPointF(hcenter + shadowOffset     , y + 16 * zoom() + shadowOffset);
        shadowPar[2] = QPointF(hcenter + b/2 + shadowOffset, y + 16 * zoom() + a/2 + shadowOffset);
        shadowPar[3] = QPointF(hcenter + shadowOffset     , y + 16 * zoom() + a + shadowOffset);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawPolygon(shadowPar, 4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QPointF par[4];
        par[0] = QPointF(hcenter - b/2, y + 16 * zoom() + a/2);
        par[1] = QPointF(hcenter      , y + 16 * zoom()      );
        par[2] = QPointF(hcenter + b/2, y + 16 * zoom() + a/2);
        par[3] = QPointF(hcenter      , y + 16 * zoom() + a  );
        canvas->drawPolygon(par, 4);
        QRectF textRect(hcenter - b/2 + a/4 + 20, y + 16 * zoom()+4 + a/8, b - a/2 - 40, a - 8 - a/4);
        canvas->drawText(textRect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString("%1?").arg(attributes.value("cond", "")));
        QBlock *left = item(0);
        Q_ASSERT_X(left != 0, "QBlock::paint()" ,"item(0) == 0. i.e. left branch of IF is nul.");
        QBlock *right = item(1);
        Q_ASSERT_X(right != 0, "QBlock::paint()" ,"item(1) == 0. i.e. right branch of IF is nul.");
        // левая линия
        QPointF line[3];
        line[0] = QPointF(hcenter - b/2, y + 16 * zoom() + a/2);
        line[1] = QPointF(left->x+left->width/2, y + 16 * zoom() + a/2);
        line[2] = QPointF(left->x+left->width/2, left->y);
        canvas->drawPolyline(line, 3);

        canvas->drawText(QPointF(hcenter - b/2 - 24*zoom(), y + 12 * zoom() + a/2), tr("Yes"));

        // правая линия
        line[0] = QPointF(hcenter + b/2, y + 16 * zoom() + a/2);
        line[1] = QPointF(right->x+right->width/2, y + 16 * zoom() + a/2);
        line[2] = QPointF(right->x+right->width/2, right->y);
        canvas->drawPolyline(line, 3);
        canvas->drawText(QPointF(hcenter + b/2 +5*zoom(), y + 12 * zoom() + a/2), tr("No"));

        // соединение
        QPointF collector[4];
        collector[0] = QPointF(left->x + left->width / 2, left->y+left->height);
        collector[1] = QPointF(left->x + left->width / 2, bottom - 8*zoom());
        collector[2] = QPointF(right->x + right->width / 2, bottom - 8*zoom());
        collector[3] = QPointF(right->x + right->width / 2, right->y+right->height);
        canvas->drawPolyline(collector, 4);
        canvas->drawLine(QLineF(hcenter, bottom-8*zoom(), hcenter, bottom+0.5));
      }
      else if(type() == "pre")
      {
        /* цикл с предусловием */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 32 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 32 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        
        // Draw shadow diamond
        QPointF shadowPar[4];
        shadowPar[0] = QPointF(hcenter - b/2 + shadowOffset, y + 32 * zoom() + a/2 + shadowOffset);
        shadowPar[1] = QPointF(hcenter + shadowOffset     , y + 32 * zoom() + shadowOffset);
        shadowPar[2] = QPointF(hcenter + b/2 + shadowOffset, y + 32 * zoom() + a/2 + shadowOffset);
        shadowPar[3] = QPointF(hcenter + shadowOffset     , y + 32 * zoom() + a + shadowOffset);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawPolygon(shadowPar, 4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QPointF par[4];
        par[0] = QPointF(hcenter - b/2, y + 32 * zoom() + a/2);
        par[1] = QPointF(hcenter      , y + 32 * zoom()      );
        par[2] = QPointF(hcenter + b/2, y + 32 * zoom() + a/2);
        par[3] = QPointF(hcenter      , y + 32 * zoom() + a  );
        canvas->drawPolygon(par, 4);
        QRectF rect(hcenter - b/2, y + 32 * zoom(), b, a);
        canvas->drawText(rect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString("%1?").arg(attributes.value("cond", "")));
        QBlock *left = item(0);
        Q_ASSERT_X(left != 0, "QBlock::paint()" ,"item(0) == 0. i.e. body of PRE-loop is nul.");
        canvas->drawLine(QLineF(hcenter,y + 32 * zoom() + a,hcenter,left->y));

//        // правая линия
        QPointF line[5];
        line[0] = QPointF(hcenter + b/2, y + 32 * zoom() + a/2);
        line[1] = QPointF(x + width - 5*zoom(), y + 32 * zoom() + a/2);
        line[2] = QPointF(x + width - 5*zoom(), bottom - 4 * zoom());
        line[3] = QPointF(hcenter, bottom - 4 * zoom());
        line[4] = QPointF(hcenter, bottom+0.5);
        canvas->drawPolyline(line, 5);
        canvas->drawText(QPointF(hcenter + 4*zoom(), y + 44 * zoom() + a), tr("Yes"));
        canvas->drawText(QPointF(hcenter + b/2 +5*zoom(), y + 28 * zoom() + a/2), tr("No"));

        // соединение
        QPointF collector[5];
        collector[0] = QPointF(hcenter, left->y+left->height);
        collector[1] = QPointF(hcenter, bottom - 28*zoom());
        collector[2] = QPointF(x + 5*zoom(), bottom - 28*zoom());
        collector[3] = QPointF(x + 5*zoom(), y + 8*zoom());
        collector[4] = QPointF(hcenter, y + 8*zoom());
        canvas->drawPolyline(collector, 5);
        QFlowChart::drawRightArrow(canvas, collector[4],
                                    QSize(12 * zoom(), 6 * zoom()));

      }
      else if(type() == "post")
      {
        /* цикл с постусловием */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 16 * zoom()));
        QBlock *left = item(0);
        Q_ASSERT_X(left != 0, "QBlock::paint()" ,"item(0) == 0. i.e. body of POST-loop is nul.");
        // верх ромба с входяящей стрелкой
        double top = left->y+left->height + 16 * zoom();

        canvas->drawLine(QLineF(hcenter,left->y+left->height,hcenter,top));

        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, top),
                                    QSize(6 * zoom(), 12 * zoom()));
        
        // Draw shadow diamond
        QPointF shadowPar[4];
        shadowPar[0] = QPointF(hcenter - b/2 + shadowOffset, top + a/2 + shadowOffset);
        shadowPar[1] = QPointF(hcenter + shadowOffset     , top + shadowOffset);
        shadowPar[2] = QPointF(hcenter + b/2 + shadowOffset, top + a/2 + shadowOffset);
        shadowPar[3] = QPointF(hcenter + shadowOffset     , top + a + shadowOffset);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawPolygon(shadowPar, 4);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QPointF par[4];
        par[0] = QPointF(hcenter - b/2, top + a/2);
        par[1] = QPointF(hcenter      , top      );
        par[2] = QPointF(hcenter + b/2, top + a/2);
        par[3] = QPointF(hcenter      , top + a  );
        canvas->drawPolygon(par, 4);
        QRectF rect(hcenter - b/2, top, b, a);
        canvas->drawText(rect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString("%1?").arg(attributes.value("cond", "")));

        canvas->drawText(QPointF(hcenter - b/2 - 24*zoom(), top - 4* zoom() + a/2), tr("Yes"));
        canvas->drawText(QPointF(hcenter  +4*zoom(), top + 16 * zoom() + a), tr("No"));

        // соединение
        QPointF collector[4];
        collector[0] = QPointF(hcenter - b/2, top + a/2);
        collector[1] = QPointF(x + 5*zoom(), top + a/2);
        collector[2] = QPointF(x + 5*zoom(), y + 8*zoom());
        collector[3] = QPointF(hcenter, y + 8*zoom());
        canvas->drawPolyline(collector, 4);
        QFlowChart::drawRightArrow(canvas, collector[3],
                                    QSize(12 * zoom(), 6 * zoom()));

        // выход
        canvas->drawLine(QLineF(hcenter, top + a, hcenter, bottom+0.5));
      }
      else if(type() == "for")
      {
        /* цикл FOR */
        canvas->drawLine(QLineF(hcenter, y-0.5, hcenter, y + 32 * zoom()));
        QFlowChart::drawBottomArrow(canvas, QPointF(hcenter, y + 32 * zoom()),
                                    QSize(6 * zoom(), 12 * zoom()));
        
        // Draw shadow hexagon
        QPointF shadowHex[6];
        shadowHex[0] = QPointF(hcenter - b/2 + shadowOffset, y + 32 * zoom() + a/2 + shadowOffset);
        shadowHex[1] = QPointF(hcenter - a/2 + shadowOffset, y + 32 * zoom() + shadowOffset);
        shadowHex[2] = QPointF(hcenter + a/2 + shadowOffset, y + 32 * zoom() + shadowOffset);
        shadowHex[3] = QPointF(hcenter + b/2 + shadowOffset, y + 32 * zoom() + a/2 + shadowOffset);
        shadowHex[4] = QPointF(hcenter + a/2 + shadowOffset, y + 32 * zoom() + a + shadowOffset);
        shadowHex[5] = QPointF(hcenter - a/2 + shadowOffset, y + 32 * zoom() + a + shadowOffset);
        canvas->setPen(Qt::NoPen);
        canvas->setBrush(shadowColor);
        canvas->drawPolygon(shadowHex, 6);
        
        // Restore pen/brush
        if(flowChart()->status() == QFlowChart::Selectable && isActive()) {
          canvas->setPen(QPen(st.selectedForeground(), lw));
          canvas->setBrush(QBrush(st.selectedBackground()));
        } else {
          canvas->setPen(QPen(st.normalForeground(), lw));
          canvas->setBrush(QBrush(st.normalBackground()));
        }
        
        QPointF hex[6];
        hex[0] = QPointF(hcenter - b/2, y + 32 * zoom() + a/2);
        hex[1] = QPointF(hcenter - a/2, y + 32 * zoom()      );
        hex[2] = QPointF(hcenter + a/2, y + 32 * zoom()      );
        hex[3] = QPointF(hcenter + b/2, y + 32 * zoom() + a/2);
        hex[4] = QPointF(hcenter + a/2, y + 32 * zoom() + a  );
        hex[5] = QPointF(hcenter - a/2, y + 32 * zoom() + a  );
        canvas->drawPolygon(hex, 6);

        QRectF rect(hcenter - b/2, y + 32 * zoom(), b, a);
        canvas->drawText(rect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString("%1 := %2...%3").arg(attributes.value("var", ""), attributes.value("from", ""), attributes.value("to", "")));
        QBlock *left = item(0);
        Q_ASSERT_X(left != 0, "QBlock::paint()" ,"item(0) == 0. i.e. body of PRE-loop is nul.");
        canvas->drawLine(QLineF(hcenter,y + 32 * zoom() + a,hcenter,left->y));

//        // правая линия
        QPointF line[5];
        line[0] = QPointF(hcenter + b/2, y + 32 * zoom() + a/2);
        line[1] = QPointF(x + width - 5*zoom(), y + 32 * zoom() + a/2);
        line[2] = QPointF(x + width - 5*zoom(), bottom - 4 * zoom());
        line[3] = QPointF(hcenter, bottom - 4 * zoom());
        line[4] = QPointF(hcenter, bottom+0.5);
        canvas->drawPolyline(line, 5);

        // соединение
        QPointF collector[5];
        collector[0] = QPointF(hcenter, left->y+left->height);
        collector[1] = QPointF(hcenter, bottom - 28*zoom());
        collector[2] = QPointF(x + 5*zoom(), bottom - 28*zoom());
        collector[3] = QPointF(x + 5*zoom(), y + 32*zoom() + a/2);
        collector[4] = QPointF(hcenter - b/2, y + 32*zoom() + a/2);
        canvas->drawPolyline(collector, 5);
        QFlowChart::drawRightArrow(canvas, collector[4],
                                    QSize(12 * zoom(), 6 * zoom()));

      }

    }
    //canvas->drawText(x+8, y+12, type());
    for(int i = 0; i < items.size(); ++i)
    {
      item(i)->paint(canvas, fontSizeInPoints);
    }
  }
}
