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


#ifndef PLUGIN_VOLUME_H
#define PLUGIN_VOLUME_H

#include <QObject>
#include "QtTraderPlugin.h"

class Volume : public QObject, public IIndicatorPlugin
{
  Q_OBJECT
    Q_INTERFACES(IIndicatorPlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:

    QString pluginName() { return QString("Volume");}
    QString pluginVersion() { return QString("0.1");}
    Entity* querySettings();
    QDialog* dialog (QWidget *dialogParent, Entity* settings);
    QList<Curve*> runIndicator (Entity* settings);
    int getMA (QString inKey, QString outKey, int type, int period);
    Curve* getMA (Entity *settings);
public slots:
    void newDataLoaded() {}
};

#endif
