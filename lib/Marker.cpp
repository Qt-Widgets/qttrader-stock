/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2010 Stefan S. Stratigakos
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

#include "Marker.h"
#include "PluginFactory.h"

#include "Entity.h"

#include <QDebug>
#include <QObject>
#include <QUuid>

Marker::Marker (QString plugin)
{
  _pluginString = plugin;
  _selected = false;
  _handleWidth = 6;
  setYAxis(QwtPlot::yRight);
  _modified = false;
  _settings = 0;
  mReadOnly = false;
  
  if (! plugin.isEmpty())
  {
    IMarkerPlugin *plug = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(plugin));
    if (plug)
    {
        _settings = plug->querySettings();
    }
  }
}

Marker::~Marker ()
{
  detach();
  if (_settings)
    delete _settings;
}

void Marker::draw (QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap, const QRectF &rect) const
{
//  qDebug() <<"Marker::draw";
  IMarkerPlugin *plug = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(_pluginString));
  if (! plug)
    return;
  plug->draw(painter, xMap, yMap, rect, (void *) this);
}

int Marker::rtti () const
{
  return Rtti_PlotUserItem;
}

int Marker::isSelected (QPoint p)
{
  int loop;
  for (loop = 0; loop < (int) _selectionArea.count(); loop++)
  {
    QRegion r = _selectionArea.at(loop);
    if (r.contains(p))
      return 1;
  }

  return 0;
}

void Marker::clearSelectionArea ()
{
  _selectionArea.clear();
}

void
Marker::appendSelectionArea (QRegion r)
{
  _selectionArea.append(r);
}

int Marker::isGrabSelected (QPoint p)
{
  int loop;
  for (loop = 0; loop < (int) _grabHandles.count(); loop++)
  {
    QRegion r = _grabHandles.at(loop);
    if (r.contains(p))
      return loop + 1;
  }

  return 0;
}

void Marker::clearGrabHandles ()
{
  _grabHandles.clear();
}

void Marker::appendGrabHandle (QRegion r)
{
  _grabHandles.append(r);
}

void Marker::setSettings (Entity *d) {
  if (_settings)
    delete _settings;
  _settings = d;
}

Entity* Marker::settings () {
  return _settings;
}

QString Marker::plugin ()
{
  return _pluginString;
}

int Marker::handleWidth ()
{
  return _handleWidth;
}

int Marker::info (QStringList &l)
{
  IMarkerPlugin *plug = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(_pluginString));
  if (! plug)
    return 0;
  int rc = plug->info(settings(), l);
  return rc;
}

int Marker::highLow (int start, int end, double &high, double &low)
{
  IMarkerPlugin *pPlugin = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(_pluginString));
  if (!pPlugin)
    return 0;
  return pPlugin->highLow(settings(), (int&)high, (int&)low, plot(), start, end);
}

int Marker::move (int &status, QPoint p)
{
  IMarkerPlugin *pPlug = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(_pluginString));
  if (! pPlug)
    return 0;

  int rc = pPlug->onMove(this, status, p);
  return rc;
}

int Marker::click (int &status, int button, QPoint p)
{
  IMarkerPlugin *pPlug = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(_pluginString));
  if (! pPlug)
    return 0;

  int rc = pPlug->onClick(this, status, p, button);
  
  return rc;
}

int Marker::create (int &status)
{
  IMarkerPlugin *pPlug = dynamic_cast<IMarkerPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(_pluginString));
  if (! pPlug)
    return 0;
  int rc = pPlug->create(this, status);
  return rc;
}
