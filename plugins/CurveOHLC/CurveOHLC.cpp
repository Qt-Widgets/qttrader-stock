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

#include "CurveOHLC.h"
#include "OHLCBar.h"
#include "curve/Curve.h"
#include "curve/CurveOHLCType.h"
#include "Global.h"


int
CurveOHLC::draw (QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &, void *c)
{
  Curve *curve = (Curve *) c;
  QwtScaleDiv *sd = curve->plot()->axisScaleDiv(QwtPlot::xBottom);
  if (! g_symbol)
  {
    qDebug() << "CurveOHLC::draw: bars missing";
    return 0;
  }

  int loop = sd->lowerBound();
  int size = sd->upperBound();
  bool ff = false;
  
  for (; loop < size; loop++)
  {
    OHLCBar *b = (OHLCBar *) curve->bar(loop);
    if (! b)
      continue;

    painter->setPen(b->color());
    
    int x = xMap.transform(loop);
    int width = xMap.transform(loop + 1) - x;
    
    int yo = yMap.transform(b->open());
    int yh = yMap.transform(b->high());
    int yl = yMap.transform(b->low());
    int yc = yMap.transform(b->close());
    
    switch ((CurveOHLCType::Key) curve->style())
    {
      case CurveOHLCType::_OHLC:
      {
        QRect rect(QPoint(x + 1, yh), QPoint(x + width - 1, yl));
        painter->drawLine (rect.center().x(), yh, rect.center().x(), yl); // draw the high/low line
        painter->drawLine (rect.left(), yo, rect.center().x(), yo); // draw the open tick
        painter->drawLine (rect.right(), yc, rect.center().x(), yc); // draw the close tick
        break;
      }
      default: // Candle
      {
        // fill all candles, color is enough. Fill/empty is redundant information.
        QRect rect(QPoint(x + 2, yo), QPoint(x + width - 2, yc));
        painter->drawLine (rect.center().x(), yh, rect.center().x(), yl);
        painter->setBrush(b->color());
        painter->drawRect(rect);
      }
    }
  }
  
  return 1;
}

int CurveOHLC::info (Curve *curve, QStringList &info, int index)
{
  if (! curve)
    return 0;
  
  OHLCBar *bar = (OHLCBar *) curve->bar(index);
  if (! bar)
    return 0;

  QString d = QString::number(bar->open(), 'f', 2);
  info << "O=" + d;


  d = QString::number(bar->high(), 'f', 2);
  info << "H=" + d;


  d = QString::number(bar->low(), 'f', 2);
  info << "L=" + d;


  d = QString::number(bar->close(), 'f', 2);
  info << "C=" + d;

  info << "Index= " + QString::number(index);

  return 1;
}

int
CurveOHLC::scalePoint (Curve *curve, QColor &color, double &value, int index)
{
  if (!curve)
    return 0;

  OHLCBar *bar = (OHLCBar *) curve->bar(index - 1); // -1 fix for alignment issue
  if (! bar)
    return 0;

  color = bar->color();
  value = bar->close();

  return 1;
}

int
CurveOHLC::highLow(Curve *curve, double &high, double &low, int start, int end)
{
  if (! curve)
    return 0;
  
  int flag = 0;
  for (int pos = start; pos <= end; pos++)
  {
    OHLCBar *r = (OHLCBar *) curve->bar(pos);
    if (! r)
      continue;

    if (! flag)
    {
      high = r->high();
      low = r->low();
      flag++;
    }
    else
    {
      if (r->high() > high)
        high = r->high();
      
      if (r->low() < low)
        low = r->low();
    }
  }

  return flag;
}

int
CurveOHLC::fill (Curve *curve, QString key1, QString key2, QString key3, QString key4, QColor color)
{
  if (key1.isEmpty() || key2.isEmpty() || key3.isEmpty() || key4.isEmpty())
    return 0;
  
  if (! g_symbol)
  {
    qDebug() << "CurveOHLC::fill: bars missing";
    return 0;
  }
  
  QList<int> keys = g_symbol->keys();
  
  for (int pos = 0; pos < keys.size(); pos++)
  {
    CBar *r = g_symbol->bar(keys.at(pos));
    
    double o = 0;
    if (! r->get(key1, o))
      continue;

    double h = 0;
    if (! r->get(key2, h))
      continue;

    double l = 0;
    if (! r->get(key3, l))
      continue;

    double c = 0;
    if (! r->get(key4, c))
      continue;
    
    curve->setBar(keys.at(pos), new OHLCBar(color, o, h, l, c));
  }

  return 1;
}

// do not remove
Q_EXPORT_PLUGIN2(curveohlc, CurveOHLC)
