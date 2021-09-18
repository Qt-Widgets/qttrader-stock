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

#include "CurveHistogram.h"
#include "CHBar.h"
#include "curve/Curve.h"
//#include "CurveHistogramType.h"
#include "Global.h"

int CurveHistogram::draw (QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &, void *c)
{
  Curve *curve = (Curve *) c;
  QwtScaleDiv *sd = curve->plot()->axisScaleDiv(QwtPlot::xBottom);
  if (! g_symbol)
  {
    qDebug() << "CurveHistogram::draw: bars missing";
    return 0;
  }

  int loop = sd->lowerBound();
  int size = sd->upperBound();
  
  for (; loop < size; loop++)
  {
    CHBar *b = (CHBar *) curve->bar(loop);
    if (! b)
      continue;

    QPolygon poly;
    int width = xMap.transform(loop + 1) - xMap.transform(loop);
    
    // base left
    int x = xMap.transform(loop);
    int by = yMap.transform(b->base());
    poly << QPoint(x, by);
    
    // value left
    int vy = yMap.transform(b->value());
    poly << QPoint(x, vy);
    
    // value right
    x += width;
    poly << QPoint(x, vy);
    
    // base right
    poly << QPoint(x, by);
    
    painter->setBrush(b->color());
    painter->drawPolygon(poly, Qt::OddEvenFill);
  }
  
  return 1;
}

int CurveHistogram::info(Curve *curve, QStringList &info, int index)
{
  CHBar *bar = (CHBar *) curve->bar(index);
  if (! bar)
    return 0;

  QString d = QString::number(bar->value(), 'f', 2);
  info << curve->label() + "=" + d;

  return 1;
}

int CurveHistogram::scalePoint (Curve *curve, QColor &color, double &value, int index)
{
  CHBar *bar = (CHBar *) curve->bar(index - 1); // -1 fix for alignment issue
  if (! bar)
    return 0;

  value = bar->value();
  color = bar->color();

  return 1;
}

int CurveHistogram::highLow(Curve *curve, double &high, double &low, int start, int end)
{
  int flag = 0;
  for (int pos = start; pos <= end; pos++)
  {
    CHBar *r = (CHBar *) curve->bar(pos);
    if (! r)
      continue;

    if (! flag)
    {
      high = r->base();
      low = r->base();
      
      if (r->value() > high)
        high = r->value();
      if (r->value() < low)
        low = r->value();

      flag++;
    }
    else
    {
      if (r->base() > high)
        high = r->base();
      if (r->base() < low)
        low = r->base();

      if (r->value() > high)
        high = r->value();
      if (r->value() < low)
        low = r->value();
    }
  }
  return flag;
}

int CurveHistogram::fill (Curve *curve, QString key,QString,QString,QString,QColor color)
{
  if (key.isEmpty())
    return 0;
  
  if (! g_symbol)
  {
    qDebug() << "CurveHistogram::fill: bars missing";
    return 0;
  }
  
  QList<int> keys = g_symbol->keys();

  for (int pos = 0; pos < keys.size(); pos++)
  {
    CBar *r = g_symbol->bar(keys.at(pos));
    
    double v = 0.0;
    if (! r->get(key, v))
      continue;

    curve->setBar(keys.at(pos), new CHBar(color, 0.0, v));
  }

  return 1;
}

// do not remove
Q_EXPORT_PLUGIN2(curvehistogram, CurveHistogram);
