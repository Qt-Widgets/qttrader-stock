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


#ifndef PLUGIN_YAHOO_HISTORY_H
#define PLUGIN_YAHOO_HISTORY_H

#include <QObject>

#include "QtTraderPlugin.h"

class YahooHistory : public QObject, public IGUIPlugin
{
  Q_OBJECT
    Q_INTERFACES(IGUIPlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:
    QString pluginName() { return QString("YahooHistory Downloader plugin");}
    QString pluginVersion() { return QString("0.1");}
    Entity* querySettings();
    Widget* create();
};

#endif
