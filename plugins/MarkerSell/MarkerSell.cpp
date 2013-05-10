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

#include "MarkerSell.h"
#include "Marker.h"
#include "plot/Plot.h"
#include "plot/PlotStatus.h"
#include "plot/PlotDateScaleDraw.h"
#include "Global.h"
#include "MarkerSellDialog.h"

#include <QtGui>


int MarkerSell::draw (QPainter *p, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &, void *m)
{
  Marker *sell = (Marker *) m;
  Entity *e = sell->settings();
  
  PlotDateScaleDraw *dsd = (PlotDateScaleDraw *) sell->plot()->axisScaleDraw(QwtPlot::xBottom);

  QVariant *date = e->get(QString("date"));
  if (! date)
  {
    qDebug() << "MarkerSell::draw: date missing";
    return 0;
  }
  
  int x = xMap.transform(dsd->dateToX(date->toDateTime()));
  if (x == -1)
    return 1;

  QVariant *price = e->get(QString("price"));
  if (! price)
  {
    qDebug() << "MarkerSell::draw: price missing";
    return 0;
  }
  
  int y = yMap.transform(price->toDouble());

  QVariant *tset = e->get(QString("color"));
  if (! tset)
  {
    qDebug() << "MarkerSell::draw: color missing";
    return 0;
  }
  QColor color(tset->toString());
  
  p->setBrush(color);

  QPolygon arrow;
  arrow.putPoints(0, 7, x, y,
                  x + 11, y - 11,
                  x + 5, y - 11,
                  x + 5, y - 21,
                  x - 5, y - 21,
                  x - 5, y - 11,
                  x - 11, y - 11);

  p->drawPolygon(arrow, Qt::OddEvenFill);

  sell->clearSelectionArea();

  sell->appendSelectionArea(QRegion(arrow));
  
  int handleWidth = sell->handleWidth();

  if (sell->selected())
  {
    sell->clearGrabHandles();

    sell->appendGrabHandle(QRegion(x - (handleWidth / 2),
                           y - handleWidth,
                           handleWidth,
                           handleWidth,
                           QRegion::Rectangle));

    p->fillRect(x - (handleWidth / 2),
                y + (handleWidth / 2),
                handleWidth,
                handleWidth,
                color);
  }
  
  return 1;
}

int MarkerSell::info (Entity *pEntity, QStringList &info)
{
  if (!pEntity)
    return 0;

  QVariant *price = pEntity->get(QString("price"));
  if (! price)
    return 0;

  QVariant *date = pEntity->get(QString("date"));
  if (! date)
    return 0;

  QDateTime dt = date->toDateTime();
  info << "D=" + dt.toString("yyyy-MM-dd");
  info << "T=" + dt.toString("HH:mm:ss");

  QString  ts = QString::number(price->toDouble(), 'f', 2);
  info << QString("Sell=") + ts;
  
  return 1;
}

int MarkerSell::onMove(Marker* pMarker, int &status, QPoint point)
{
  if (!pMarker)
    return 0;
    
  switch (status)
  {
    case PlotStatus::_MOVE:
    {
      Entity *pEntity = pMarker->settings();
      
      QVariant *price = pEntity->get(QString("price"));
      if (! price)
        return 0;

      QVariant *date = pEntity->get(QString("date"));
      if (! date)
        return 0;
      
      QwtScaleMap map = pMarker->plot()->canvasMap(QwtPlot::xBottom);
      int x = map.invTransform((double) point.x());

      if (! g_symbol)
        return 0;
      Bar *tbar = g_symbol->bar(x);
      if (! tbar)
        return 0;
      date->setValue(tbar->date());

      map = pMarker->plot()->canvasMap(QwtPlot::yRight);
      price->setValue(map.invTransform((double) point.y()));
      
      pMarker->setModified(TRUE);

      pMarker->plot()->replot();
      break;
    }
    default:
      break;
  }
  
  return 1;
}

int
MarkerSell::highLow (Entity* pEntity, int &high, int &low, QwtPlot*, int, int)
{
  if (!pEntity)
    return 0;

  QVariant *price = pEntity->get(QString("price"));
  if (! price)
    return 0;

  high = price->toDouble();
  low = price->toDouble();

  return 1;
}

int
MarkerSell::onClick(Marker* pMarker, int &status, QPoint point, int button)
{
  if (!pMarker)
    return 0;
  
  switch ((PlotStatus::Key) status)
  {
    case PlotStatus::_SELECTED:
    {
      switch (button)
      {
        case Qt::LeftButton:
          if (pMarker->isGrabSelected(point))
          {
            status = PlotStatus::_MOVE;
            return 1;
          }

          if (! pMarker->isSelected(point))
          {
            status = PlotStatus::_NONE;
            pMarker->setSelected(FALSE);

            Plot *tplot = (Plot *) pMarker->plot();
            tplot->unselectMarker();

            pMarker->plot()->replot();
            return 1;
          }
          break;
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
            pMarker->setSelected(TRUE);

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

int MarkerSell::create (Marker* pMarker, int &status)
{
  if (!pMarker)
    return 0;
  
  status = PlotStatus::_MOVE;
  pMarker->setSelected(TRUE);
  emit signalMessage(QObject::tr("Place Sell marker"));
  return 1;
}

QDialog* MarkerSell::getDialog(QWidget *dialogParent, Entity* settings)
{
  if (!dialogParent)
  {
    qDebug() << "MarkerBuy::dialog: invalid parent";
    return 0;
  }

  if (!settings)
  {
    qDebug() << "MarkerBuy::dialog: invalid settings";
    return 0;
  }

  MarkerSellDialog *dialog = new MarkerSellDialog(dialogParent);
  dialog->setGUI(settings);
  return dialog;
}

Entity* MarkerSell::querySettings ()
{
  Entity *pEntity = new Entity;
  pEntity->set(QString("plot"), new QVariant(QString()));
  pEntity->set(QString("type"), new QVariant(QString("marker")));
  pEntity->set(QString("symbol"), new QVariant(QString()));
  pEntity->set(QString("plugin"), new QVariant(QString("MarkerSell")));
  pEntity->set(QString("date"), new QVariant(QDateTime::currentDateTime()));
  pEntity->set(QString("price"), new QVariant(0.0));
  pEntity->set(QString("color"), new QVariant(QString("red")));
  return pEntity;
}

// do not remove
Q_EXPORT_PLUGIN2(markersell, MarkerSell)
