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

#ifndef PLUGIN_MARKER_TLINE_H
#define PLUGIN_MARKER_TLINE_H

#include <QObject>
#include "QtTraderPlugin.h"

class MarkerTLine : public QObject, public IMarkerPlugin
{
  Q_OBJECT
    Q_INTERFACES(IMarkerPlugin)
    Q_INTERFACES(QtTraderPlugin)
  
  signals:
    void signalMessage (QString);
  
  public:
    QString pluginName() { return QString("Trendline");}
    QString pluginVersion() { return QString("0.1");}

    int onMove(Marker* pMarker, int &status, QPoint point);
    int onClick(Marker* pMarker, int &status, QPoint point, int button);

    int info(Entity *pEntity, QStringList &info);
    int highLow(Entity* pEntity, int &high, int &low, QwtPlot* pPlot, int start, int end);
    int create(Marker* pMarker, int &status);

    Entity* querySettings();
    QDialog* getDialog(QWidget *dialogParent, Entity* settings);

    int draw (QPainter *, const QwtScaleMap &, const QwtScaleMap &, const QRectF &, void *);

};

#endif
