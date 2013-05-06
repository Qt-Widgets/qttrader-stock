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


#ifndef DBSTOCK_H
#define DBSTOCK_H

#include <QObject>
#include <QtSql>
#include <QStringList>
#include "QtTraderPlugin.h"


class DBStock : public QObject, IDBPlugin
{
  Q_OBJECT
    Q_INTERFACES(IDBPlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:

    QString pluginName() { return QString("DB Stock");}
    QString pluginVersion() { return QString("0.1");}
    Entity* querySettings();
    int init ();
    int setBars (Bars *symbol);
    int getBars (Bars *symbol);
    int newTable (Bars *symbol);
    QList<Bars> search (QString );
    int newSymbol (Bars *) { return 0; }
    int getSymbol (Bars *) { return 0; }
    int deleteSymbol (Bars *){ return 0; }
    int setName (Bars *) { return 0; }
    void transaction();
    void commit();

    QDateTime getMaxDate (Bars *);

  private:
    QSqlDatabase _db;
};

#endif
