/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2007 Stefan S. Stratigakos
 *  Copyright (C) 2012-2013 Mattias Johansson
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

#include "MarkerTLineDialog.h"
#include "MarkerTLine.h"
#include "Strip.h"
#include "Marker.h"
#include "Plot.h"
#include "PlotStatus.h"
#include "PlotDateScaleDraw.h"
#include "Global.h"

#include <QtGui>

int MarkerTLine::draw (QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &, void *m)
{

  Marker *tline = (Marker *) m;
  
  Entity *e = tline->settings();
  
  QVariant *date = e->get(QString("date"));
  if (! date)
    return 0;
  
  QVariant *date2 = e->get(QString("date2"));
  if (! date2)
    return 0;

  QVariant *price = e->get(QString("price"));
  if (! price)
    return 0;

  QVariant *price2 = e->get(QString("price2"));
  if (! price2)
    return 0;

  QVariant *tset = e->get(QString("color"));
  if (! tset)
    return 0;
  QColor color(tset->toString());

  QVariant *extend = e->get(QString("extend"));
  if (! extend)
    return 0;

  PlotDateScaleDraw *dsd = (PlotDateScaleDraw *) tline->plot()->axisScaleDraw(QwtPlot::xBottom);
  int x = xMap.transform(dsd->dateToX(date->toDateTime()));
  int x2 = xMap.transform(dsd->dateToX(date2->toDateTime()));
  int y = yMap.transform(price->toDouble());
  int y2 = yMap.transform(price2->toDouble());

  p->setPen(color);

  p->drawLine (x, y, x2, y2);

  // save old values;
  int tx2 = x2;
  int ty2 = y2;
  int tx = x;
  int ty = y;

  if (extend->toBool())
  {
    int ydiff = y - y2;
    int xdiff = x2 - x;
    if (xdiff > 0)
    {
      while (x2 < p->window().width())
      {
        x = x2;
        y = y2;
        x2 = x2 + xdiff;
        y2 = y2 - ydiff;
        p->drawLine (x, y, x2, y2);
      }
    }
  }

  // store the selectable area the line occupies
  tline->clearSelectionArea();

  QPolygon array;
  array.putPoints(0, 4, tx, ty - 4, tx, ty + 4, x2, y2 + 4, x2, y2 - 4);
  tline->appendSelectionArea(QRegion(array));

  if (tline->selected())
  {
    int handleWidth = tline->handleWidth();
    
    tline->clearGrabHandles();

    tline->appendGrabHandle(QRegion(tx,
                            ty - (handleWidth / 2),
                            handleWidth,
                            handleWidth,
                            QRegion::Rectangle));

    p->fillRect(tx,
                ty - (handleWidth / 2),
                handleWidth,
                handleWidth,
                color);

    tline->appendGrabHandle(QRegion(tx2,
                            ty2 - (handleWidth / 2),
                            handleWidth,
                            handleWidth,
                            QRegion::Rectangle));

    p->fillRect(tx2,
                ty2 - (handleWidth / 2),
                handleWidth,
                handleWidth,
                color);
  }
  
  return 1;
}

int MarkerTLine::info (Entity *pEntity, QStringList &info)
{

  QVariant *price = pEntity->get(QString("price"));
  if (! price)
    return 0;

  QVariant *date = pEntity->get(QString("date"));
  if (! date)
    return 0;
  
  QVariant *price2 = pEntity->get(QString("price2"));
  if (! price2)
    return 0;

  QVariant *date2 = pEntity->get(QString("date2"));
  if (! date2)
    return 0;

  info<< tr("TLine");

  QDateTime dt = date->toDateTime();
  info<< tr("Start Date") + "=" + dt.toString("yyyy-MM-dd");
  info<< tr("Start Time") + "=" + dt.toString("HH:mm:ss");

  dt = date2->toDateTime();
  info<< tr("End Date") + "=" + dt.toString("yyyy-MM-dd");
  info<< tr("End Time") + "=" + dt.toString("HH:mm:ss");

  Strip strip;
  QString ts;
  strip.strip(price->toDouble(), 4, ts);
  info<< tr("Start Price") + "=" + ts;

  strip.strip(price2->toDouble(), 4, ts);
  info<< tr("End Price") + "=" + ts;
  
  return 1;
}

int MarkerTLine::onMove(Marker* pMarker, int &status, QPoint point)
{
  
  Entity *pEntity = pMarker->settings();
  
  switch (status)
  {
    case PlotStatus::_MOVE:
    case PlotStatus::_CREATE_MOVE:
    {
      QVariant *price = pEntity->get(QString("price"));
      if (! price)
        return 0;

      QVariant *date = pEntity->get(QString("date"));
      if (! date)
        return 0;
  
      QVariant *price2 = pEntity->get(QString("price2"));
      if (! price2)
        return 0;

      QVariant *date2 = pEntity->get(QString("date2"));
      if (! date2)
        return 0;
      
      QwtScaleMap map = pMarker->plot()->canvasMap(QwtPlot::xBottom);
      int x = map.invTransform((double) point.x());

      date->setValue(g_symbol->date(x));
      map = pMarker->plot()->canvasMap(QwtPlot::yRight);
      price->setValue(map.invTransform((double) point.y()));

      if (status == PlotStatus::_CREATE_MOVE)
      {
        price2->setValue(price->toDouble());
        date2->setValue(g_symbol->date(x));
      }

      pMarker->plot()->replot();

      pMarker->setModified(true);
      break;
    }
    case PlotStatus::_MOVE2:
    {
      QVariant *price2 = pEntity->get(QString("price2"));
      if (! price2)
        return 0;

      QVariant *date2 = pEntity->get(QString("date2"));
      if (! date2)
        return 0;

      QwtScaleMap map = pMarker->plot()->canvasMap(QwtPlot::xBottom);
      int x = map.invTransform((double) point.x());

      date2->setValue(g_symbol->date(x));
      map = pMarker->plot()->canvasMap(QwtPlot::yRight);
      price2->setValue(map.invTransform((double) point.y()));

      pMarker->plot()->replot();

      pMarker->setModified(true);
      break;
    }
    default:
      break;
  }
  
  return 1;
}

int MarkerTLine::highLow (Entity* pEntity, int &high, int &low, QwtPlot* pPlot, int start, int end)
{
  PlotDateScaleDraw *pDateScaleDraw = (PlotDateScaleDraw *) pPlot->axisScaleDraw(QwtPlot::xBottom);
  if (! pDateScaleDraw)
    return 1;

  QVariant *date = pEntity->get(QString("date"));
  if (! date)
    return 0;
  
  QVariant *date2 = pEntity->get(QString("date2"));
  if (! date2)
    return 0;

  QVariant *price = pEntity->get(QString("price"));
  if (! price)
    return 0;

  QVariant *price2 = pEntity->get(QString("price2"));
  if (! price2)
    return 0;
  
  int x = pDateScaleDraw->dateToX(date->toDateTime());
  if (x >= start && x <= end)
  {
    high = price->toDouble();
    double t = price2->toDouble();
    if (t > high)
      high = t;
    low = price->toDouble();
    if (t < low)
      low = t;
    
    return 1;
  }

  int x2 = pDateScaleDraw->dateToX(date2->toDateTime());
  if (x2 >= start && x2 <= end)
  {
    high = price->toDouble();
    double t = price2->toDouble();
    if (t > high)
      high = t;
    low = price->toDouble();
    if (t < low)
      low = t;
    
    return 1;
  }

  return 1;
}

int MarkerTLine::onClick(Marker* pMarker, int &status, QPoint point, int button)
{
//  qDebug() << "MarkerTLine::click";
  switch (status)
  {
    case PlotStatus::_SELECTED:
    {
      switch (button)
      {
        case Qt::LeftButton:
        {
          int grab = pMarker->isGrabSelected(point);
          if (grab)
          {
            status = PlotStatus::_MOVE;
            if (grab == 2)
              status = PlotStatus::_MOVE2;
            return 1;
          }

          if (! pMarker->isSelected(point))
          {
            status = PlotStatus::_NONE;
            pMarker->setSelected(false);
            Plot *tplot = (Plot *) pMarker->plot();
            tplot->unselectMarker();
            pMarker->plot()->replot();
            return 1;
          }
          break;
        }
        case Qt::RightButton:
        {
          Plot *p = (Plot *) pMarker->plot();
          p->showMarkerMenu();
          break;
        }
        default:
          break;
      }

      break;
    }
    case PlotStatus::_MOVE:
    case PlotStatus::_CREATE_MOVE:
    {
      switch (button)
      {
        case Qt::LeftButton:
          if (status == PlotStatus::_CREATE_MOVE)
          {
            status = PlotStatus::_MOVE2;
            emit signalMessage(tr("Select TLine ending point..."));
            return 1;
          }
          status = PlotStatus::_SELECTED;
          return 1;
        default:
          break;
      }

      break;
    }
    case PlotStatus::_MOVE2:
    {
      switch (button)
      {
        case Qt::LeftButton:
          status = PlotStatus::_SELECTED;
          return 1;
        default:
          break;
      }

      break;
    }
    default: // _None
    {
      switch (button)
      {
        case Qt::LeftButton:
        {
          if (pMarker->isSelected(point))
          {
            status = PlotStatus::_SELECTED;
            pMarker->setSelected(true);
            Plot *tplot = (Plot *) pMarker->plot();
            
            tplot->selectMarker(pMarker->ID());
            pMarker->plot()->replot();
            return 1;
          }
          break;
        }
        default:
          break;
      }

      break;
    }
  }
  
  return 1;
}

int MarkerTLine::create(Marker* pMarker, int &status)
{  
  status = PlotStatus::_CREATE_MOVE;
  pMarker->setSelected(true);
  emit signalMessage(tr("Place TLine marker..."));
  return 1;
}

Entity* MarkerTLine::querySettings()
{
  Entity *e = new Entity;
  e->set(QString("plot"), new QVariant(QString()));
  e->set(QString("type"), new QVariant(QString("marker")));
  e->set(QString("symbol"), new QVariant(QString()));
  e->set(QString("plugin"), new QVariant(QString("MarkerTLine")));
  e->set(QString("date"), new QVariant(QDateTime::currentDateTime()));
  e->set(QString("price"), new QVariant(0.0));
  e->set(QString("date2"), new QVariant(QDateTime::currentDateTime()));
  e->set(QString("price2"), new QVariant(0.0));
  e->set(QString("color"), new QVariant(QString("red")));
  e->set(QString("extend"), new QVariant(false));
  return e;
}

QDialog* MarkerTLine::getDialog(QWidget* dialogParent, Entity* settings)
{
    if (!dialogParent || !settings){
      qDebug() << "MarkerTLine::getDialog: invalid arguments";
      return 0;
    }

    MarkerTLineDialog *dialog = new MarkerTLineDialog(dialogParent);
    dialog->setGUI(settings);
    return dialog;
}

// do not remove
Q_EXPORT_PLUGIN2(markertline, MarkerTLine)
