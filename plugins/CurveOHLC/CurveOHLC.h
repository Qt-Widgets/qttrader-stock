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


#ifndef PLUGIN_CURVE_OHLC_H
#define PLUGIN_CURVE_OHLC_H

#include <QObject>

#include "QtTraderPlugin.h"

class CurveOHLC : public QObject, public ICurvePlugin
{
  Q_OBJECT
    Q_INTERFACES(ICurvePlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:

    QString pluginName() { return QString("Curve OHLC");}
    QString pluginVersion() { return QString("0.1");}

    int draw (QPainter *, const QwtScaleMap &, const QwtScaleMap &, const QRectF &, void *);
    int info (Curve *curve, QStringList &info, int index);
    int scalePoint (Curve *curve, QColor &color, double &value, int index);
    int highLow (Curve *curve, double &high, double &low, int start, int end);
    int fill (Curve *curve, QString key1, QString key2, QString key3, QString key4, QColor color);
};

#endif
