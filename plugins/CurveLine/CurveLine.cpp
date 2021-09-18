/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2007 Stefan S. Stratigakos
 *  Copyright (C) 2013 Mattias Johansson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include <qwt_scale_div.h>
#include <QtGui>

#include "CurveLine.h"
#include "CurveLineBar.h"
#include "curve/Curve.h"
#include "curve/CurveLineType.h"
#include "Global.h"

int
CurveLine::draw (QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &, void *c)
{
  Curve *curve = (Curve *) c;
  QwtScaleDiv *sd = curve->plot()->axisScaleDiv(QwtPlot::xBottom);
  if (! g_symbol)
  {
    qDebug() << "CurveLine::draw: bars missing";
    return 0;
  }

  int loop = sd->lowerBound();
  int size = sd->upperBound();

//  painter->setRenderHint(QPainter::Antialiasing, true);

  QPen tpen = painter->pen();
  tpen.setWidth(curve->pen());
  tpen.setColor(curve->color());

  CurveLineType clt;
  int style = curve->style();
  switch ((CurveLineType::Key) style)
  {
    case CurveLineType::_DASH:
      tpen.setStyle((Qt::PenStyle) Qt::DashLine);
      break;
    case CurveLineType::_DOT:
      tpen.setStyle((Qt::PenStyle) Qt::DotLine);
      break;
    default:
      tpen.setStyle((Qt::PenStyle) Qt::SolidLine);
      break;
  }

  painter->setPen(tpen);
  
  switch ((CurveLineType::Key) style)
  {
    case CurveLineType::_DASH:
    case CurveLineType::_SOLID:
    {
      for (; loop < size; loop++)
      {
        CurveLineBar *yb = (CurveLineBar *) curve->bar(loop - 1);
        if (! yb)
          continue;

        CurveLineBar *b = (CurveLineBar *) curve->bar(loop);
        if (! b)
          continue;

        int x = xMap.transform(loop - 1);
        int x2 = xMap.transform(loop);
        int y = yMap.transform(yb->value());
        int y2 = yMap.transform(b->value());

        painter->drawLine (x, y, x2, y2);
      }
      break;
    }
    default:
    {
      for (; loop < size; loop++)
      {
        CurveLineBar *b = (CurveLineBar *) curve->bar(loop);
        if (! b)
          continue;

        int x = xMap.transform(loop);
        int y = yMap.transform(b->value());

        painter->drawPoint (x, y);
      }
      break;
    }
  }
  
  return 1;
}

int CurveLine::info (Curve *curve, QStringList &info, int index)
{
  CurveLineBar *bar = (CurveLineBar *) curve->bar(index);
  if (! bar)
    return 0;

  QString d = QString::number(bar->value(), 'f', 2);
  info << curve->label() + "=" + d;

  return 1;
}

int
CurveLine::scalePoint (Curve *curve, QColor &color, double &value, int index)
{
  if (! curve)
    return 0;
  
  CurveLineBar *bar = (CurveLineBar *) curve->bar(index);
  if (! bar)
    return 0;
  
  value = bar->value();
  color = curve->color();

  return 1;
}

int CurveLine::highLow (Curve *curve, double &high, double &low, int start, int end)
{
  if (!curve)
    return 0;

  int flag = 0;
  for (int pos = start; pos <= end; pos++)
  {
    CurveLineBar *r = (CurveLineBar *) curve->bar(pos);
    if (! r)
      continue;

    if (! flag)
    {
      high = r->value();
      low = r->value();
      flag++;
    }
    else
    {
      if (r->value() > high)
        high = r->value();
      
      if (r->value() < low)
        low = r->value();
    }
  }
  return flag;
}

int CurveLine::fill (Curve *curve, QString key,QString,QString,QString,QColor)
{
    if (!curve || key.isEmpty())
    return 0;
  
  if (! g_symbol)
  {
    qDebug() << "CurveLine::fill: bars missing";
    return 0;
  }
  
  QList<int> keys = g_symbol->keys();
  
  for (int pos = 0; pos < keys.size(); pos++)
  {
    CBar *r = g_symbol->bar(keys.at(pos));
    
//qDebug() << "CurveLine::fill:" << k1 << QString::number(pos);
    
    double v = 0;
    if (! r->get(key, v))
      continue;

    curve->setBar(keys.at(pos), new CurveLineBar(v));
  }

  return 1;
}

// do not remove
Q_EXPORT_PLUGIN2(curveline, CurveLine)
