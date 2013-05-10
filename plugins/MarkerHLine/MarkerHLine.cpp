/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2007 Stefan S. Stratigakos
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


#include "MarkerHLineDialog.h"
#include "MarkerHLine.h"
#include "Marker.h"
#include "plot/Plot.h"
#include "plot/PlotStatus.h"

#include <QtGui>

int MarkerHLine::draw (QPainter *p, const QwtScaleMap &, const QwtScaleMap &yMap, const QRectF &, void *m)
{
  Marker *hl = (Marker *) m;
  Entity *e = hl->settings();
  
  QVariant *price = e->get(QString("price"));
  if (! price)
  {
    qDebug() << "MarkerHLine::draw: price missing";
    return 0;
  }
  
  QVariant *tset = e->get(QString("color"));
  if (! tset)
  {
    qDebug() << "MarkerHLine::draw: color missing";
    return 0;
  }
  QColor color(tset->toString());
  
  p->setPen(color);

  int y = yMap.transform(price->toDouble());

  // test start
  QString ts = QString::number(price->toDouble(), 'f', 2);
  QString s = " " + ts; // prepend space so we can clearly read text
  QFontMetrics fm = p->fontMetrics();
  QRect rc = p->boundingRect(0, y - (fm.height() / 2), 1, 1, 0, s);
  p->fillRect(rc, hl->plot()->canvasBackground()); // fill in behind text first
  p->drawText(rc, s); // draw text
  p->drawRect(rc); // draw a nice little box around text

  p->drawLine (rc.width(), y, p->window().width(), y);

  hl->clearSelectionArea();

  QPolygon array;
  array.putPoints(0,
                  4,
                  0,
                  y - 4,
                  0,
                  y + 4,
                  p->window().width(),
                  y + 4,
                  p->window().width(),
                  y - 4);

  hl->appendSelectionArea(QRegion(array));

  if (hl->selected())
  {
    hl->clearGrabHandles();

    int hw = hl->handleWidth();
    int t = (int) p->window().width() / 4;
    int loop;
    for (loop = 0; loop < 5; loop++)
    {
      hl->appendGrabHandle(QRegion(t * loop,
                                   y - (hw / 2),
                                   hw,
                                   hw,
                                   QRegion::Rectangle));

      p->fillRect(t * loop,
                  y - (hw / 2),
                  hw,
                  hw,
                  color);
    }
  }
  
  return 1;
}

int MarkerHLine::info (Entity *pEntity, QStringList &info)
{ 
  QVariant *price = pEntity->get(QString("price"));
  if (! price)
    return 0;

  QString  ts = QString::number(price->toDouble(), 'f', 2);
  info << QString("HLine=") + ts;
  
  return 1;
}

int MarkerHLine::onMove(Marker* pMarker, int &status, QPoint point)
{
  Entity *e = pMarker->settings();
  
  switch (status)
  {
    case PlotStatus::_MOVE:
    {
      QVariant *price = e->get(QString("price"));
      if (! price)
        return 0;
      
      QwtScaleMap map = pMarker->plot()->canvasMap(QwtPlot::yRight);
      price->setValue(map.invTransform((double) point.y()));

      pMarker->plot()->replot();

      pMarker->setModified(TRUE);
      break;
    }
    default:
      break;
  }
  
  return 1;
}

int MarkerHLine::highLow (Entity* pEntity, int &high, int &low, QwtPlot* /*pPlot*/, int /*start*/, int /*end*/)
{
  QVariant *price = pEntity->get(QString("price"));
  if (! price)
    return 0;

  high = price->toDouble();
  low = price->toDouble();

  return 1;
}

int MarkerHLine::onClick(Marker* pMarker, int &status, QPoint point, int button)
{
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

int MarkerHLine::create(Marker* pMarker, int &status)
{
  status = PlotStatus::_MOVE;
  pMarker->setSelected(TRUE);
  emit signalMessage(QObject::tr("Place HLine marker"));
  return 1;
}

Entity* MarkerHLine::querySettings ()
{
  Entity *e = new Entity;
  e->set(QString("plot"), new QVariant(QString()));
  e->set(QString("type"), new QVariant(QString("marker")));
  e->set(QString("symbol"), new QVariant(QString()));
  e->set(QString("plugin"), new QVariant(QString("MarkerHLine")));
  e->set(QString("price"), new QVariant(0.0));
  e->set(QString("color"), new QVariant(QString("red")));
  return e;
}

QDialog* MarkerHLine::getDialog(QWidget* dialogParent, Entity* settings)
{
    if (!dialogParent || !settings){
      qDebug() << "MarkerHLine::getDialog: invalid arguments";
      return 0;
    }

    MarkerHLineDialog *dialog = new MarkerHLineDialog(dialogParent);
    dialog->setGUI(settings);
    return dialog;
}

// do not remove
Q_EXPORT_PLUGIN2(markerhline, MarkerHLine)
