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

#include <QtGui>
#include "Database.h"
#include "DateRange.h"
#include "bar/BarLength.h"
#include "bar/BarType.h"

int Database::init ()
{
  _db = QSqlDatabase::database("quote");
  if (! _db.isOpen())
  {
    QString s = QDir::homePath() + "/QtTrader/db/";
    QDir dir;
    if (! dir.mkpath(s))
    {
      qDebug() << "Database::init: error creating database" << s;
      return 0;
    }
    
    s.append("quote");
    
    _db = QSqlDatabase::addDatabase("QSQLITE", "quote");
    _db.setHostName("QtTrader");
    _db.setDatabaseName(s);
    _db.setUserName("QtTrader");
    _db.setPassword("QtTrader");
    if (! _db.open())
    {
      qDebug() << "Database::init:" << _db.lastError().text();
      return 0;
    }
  }

  QSqlQuery q(_db);
  QString s = "CREATE TABLE IF NOT EXISTS symbolIndex (";
  s.append("a INTEGER PRIMARY KEY"); // auto increment index
  s.append(", name TEXT"); // entity name
  s.append(", key TEXT"); // attribute
  s.append(", value TEXT"); // value
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::init:" << _db.lastError().text();
    return 0;
  }
  
  return 1;
}

int Database::getBars (Bars *bd)
{
  if (! bd)
    return 0;

  int length = bd->barLength();
  if (length == -1)
  {
    qDebug() << "Database::getBars: invalid length";
    return 0;
  }
  
  if (! getSymbol(bd))
    return 0;

  if (! init())
    return 0;

  // get last date in db
  QDateTime endDate = getMaxDate(bd);
  if (! endDate.isValid())
    return 0;

  DateRange dr;
  //Don't bother to load less the all values into memory
  //All computers are bound to be able handle this in year 2012
  //If ever porting to a mobile device, change at that point
  //bd->plotRange()
  QDateTime startDate = dr.interval(endDate, DateRange::_ALL);
  if (! startDate.isValid())
  {
    qDebug() << "Database::getBars: invalid range";
    return 0;
  }

  QSqlQuery q(_db);
  QString s = "SELECT date,open,high,low,close,volume";
  s.append(" FROM " + bd->table());
  s.append(" WHERE date >=" + startDate.toString("yyyyMMddHHmmss"));
  s.append(" AND date <=" + endDate.toString("yyyyMMddHHmmss"));
  s.append(" ORDER BY date ASC");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::getBars:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  BarType bt;
  BarLength bl;
  QDateTime isDate, ieDate;
  CBar *bar = 0;
  while (q.next())
  {
    QDateTime lastDate = QDateTime::fromString(q.value(0).toString(), "yyyyMMddHHmmss");

    // is date greater than current bar range?
    if (lastDate >= ieDate || ! bar)
    {
      // save old bar
      if (bar)
        bd->setBar(bd->bars(), bar);

      // create new bar
      bl.interval(lastDate, bd->barLength(), isDate, ieDate);

      bar = new CBar;
      bar->setDate(lastDate);
      bar->set(bt.indexToString(BarType::_OPEN), q.value(1).toDouble());
      bar->set(bt.indexToString(BarType::_HIGH), q.value(2).toDouble());
      bar->set(bt.indexToString(BarType::_LOW), q.value(3).toDouble());
      bar->set(bt.indexToString(BarType::_CLOSE), q.value(4).toDouble());
      bar->set(bt.indexToString(BarType::_VOLUME), q.value(5).toDouble());
    }
    else
    {
      double v = q.value(2).toDouble();
      double v2;
      bar->get(bt.indexToString(BarType::_HIGH), v2);
      if (v > v2)
        bar->set(bt.indexToString(BarType::_HIGH), v);

      v = q.value(3).toDouble();
      bar->get(bt.indexToString(BarType::_LOW), v2);
      if (v < v2)
        bar->set(bt.indexToString(BarType::_LOW), v);

      bar->set(bt.indexToString(BarType::_CLOSE), q.value(4).toDouble());

      v = q.value(5).toDouble();
      bar->get(bt.indexToString(BarType::_VOLUME), v2);
      v += v2;
      bar->set(bt.indexToString(BarType::_VOLUME), v);
    }
  }

  // save any left over bar
  if (bar)
    bd->setBar(bd->bars(), bar);

  return 1;
}

int Database::setBars (Bars *symbol)
{
  if (! symbol)
    return 0;

  if (! getSymbol(symbol))
  {
    if (! newSymbol(symbol))
      return 0;
  }
  else
  {
    // check if we need to save the name
    if (! symbol->name().isEmpty())
      setName(symbol);
  }

  if (! init())
    return 0;

  _db.transaction();

  QSqlQuery q(_db);

  QList<int> keys = symbol->keys();

  BarType bt;
  int loop = 0;
  for (; loop < keys.size(); loop++)
  {
    CBar *bar = symbol->bar(keys.at(loop));

    QDateTime dt = bar->date();

    QString date = dt.toString("yyyyMMddHHmmss");

    // first check if record exists so we know to do an update or insert
    QString s = "SELECT date FROM " + symbol->table() + " WHERE date =" + date;
    q.exec(s);
    if (q.lastError().isValid())
    {
      qDebug() << "Database::setBars:" << q.lastError().text();
      qDebug() << s;
      continue;
    }

    if (q.next()) // record exists, use update
    {
      s = "UPDATE " + symbol->table() + " SET ";

      QStringList tl;
      double v = 0;
      if (bar->get(bt.indexToString(BarType::_OPEN), v))
        tl << "open=" + QString::number(v);

      v = 0;
      if (bar->get(bt.indexToString(BarType::_HIGH), v))
        tl << "high=" + QString::number(v);

      v = 0;
      if (bar->get(bt.indexToString(BarType::_LOW), v))
        tl << "low=" + QString::number(v);

      v = 0;
      if (bar->get(bt.indexToString(BarType::_CLOSE), v))
        tl << "close=" + QString::number(v);

      v = 0;
      if (bar->get(bt.indexToString(BarType::_VOLUME), v))
        tl << "volume=" + QString::number(v);

      s.append(tl.join(","));
      s.append(" WHERE date=" + date);
    }
    else // new record, use insert
    {
      QStringList tl;
      s = "INSERT INTO " + symbol->table() + " (date,open,high,low,close,volume) VALUES(";

      tl << date;

      double v = 0;
      bar->get(bt.indexToString(BarType::_OPEN), v);
      tl << QString::number(v);

      v = 0;
      bar->get(bt.indexToString(BarType::_HIGH), v);
      tl << QString::number(v);

      v = 0;
      bar->get(bt.indexToString(BarType::_LOW), v);
      tl << QString::number(v);

      v = 0;
      bar->get(bt.indexToString(BarType::_CLOSE), v);
      tl << QString::number(v);

      v = 0;
      bar->get(bt.indexToString(BarType::_VOLUME), v);
      tl << QString::number(v);

      s.append(tl.join(","));
      s.append(")");
    }

    q.exec(s);
    if (q.lastError().isValid())
    {
      qDebug() << "Database::setBars:" << q.lastError().text();
      qDebug() << s;
      continue;
    }
  }
  _db.commit();
  return 1;
}

int Database::newSymbol (Bars *symbol)
{
  if (symbol->ticker().isEmpty() || symbol->exchange().isEmpty())
  {
    qDebug() << "Database::newSymbol: invalid SYMBOL" << symbol->symbol();
    return 0;
  }
  
  // set default name to symbol
  QString name = symbol->name();
  if (name.isEmpty())
    symbol->setName(symbol->symbol());
  
  QString table = "Q" + QUuid::createUuid().toString();
  table.remove("{");
  table.remove("}");
  table.remove("-");
  symbol->setTable(table);

  // add new symbol entry into the symbolIndex table

  QSqlQuery q(_db);

  // add table
  QString s = "INSERT INTO symbolIndex VALUES(";
  s.append("NULL"); // auto increment
  s.append(",'" + symbol->symbol() + "'");
  s.append(",'table'");
  s.append(",'" + symbol->table() + "'");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  // add SYMBOL
  s = "INSERT INTO symbolIndex VALUES(";
  s.append("NULL"); // auto increment
  s.append(",'" + symbol->symbol() + "'");
  s.append(",'symbol'");
  s.append(",'" + symbol->ticker() + "'");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  // add EXCHANGE
  s = "INSERT INTO symbolIndex VALUES(";
  s.append("NULL"); // auto increment
  s.append(",'" + symbol->symbol() + "'");
  s.append(",'exchange'");
  s.append(",'" + symbol->exchange() + "'");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  // add NAME
  s = "INSERT INTO symbolIndex VALUES(";
  s.append("NULL"); // auto increment
  s.append(",'" + symbol->symbol() + "'");
  s.append(",'name'");
  s.append(",'" + symbol->name() + "'");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  // add TYPE
  s = "INSERT INTO symbolIndex VALUES(";
  s.append("NULL"); // auto increment
  s.append(",'" + symbol->symbol() + "'");
  s.append(",'type'");
  s.append(",'" + symbol->symbolType() + "'");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  // add plugin
  s = "INSERT INTO symbolIndex VALUES(";
  s.append("NULL"); // auto increment
  s.append(",'" + symbol->symbol() + "'");
  s.append(",'plugin'");
  s.append(",'Obsolete'");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  if (! init())
    return 0;

  if (symbol->table().isEmpty())
  {
    qDebug() << "Database::newTable: invalid table";
    return 0;
  }

  // create new table
  s = "CREATE TABLE IF NOT EXISTS " + symbol->table() + " (";
  s.append("date INT PRIMARY KEY UNIQUE");
  s.append(", open REAL");
  s.append(", high REAL");
  s.append(", low REAL");
  s.append(", close REAL");
  s.append(", volume INT");
  s.append(")");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::newTable:" + q.lastError().text();
    return 0;
  }

  return 1;
}

int Database::getSymbol (Bars *symbol)
{
  QSqlQuery q(_db);
  QString s = "SELECT key,value FROM symbolIndex WHERE name='" + symbol->symbol() + "'";
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::getSymbol:" + q.lastError().text();
    qDebug() << s;
    return 0;
  }

  QStringList keys;
  keys << "type" << "name" << "table";
  
  int rc = 0;
  while (q.next())
  {
    rc++;
    
    switch (keys.indexOf(q.value(0).toString()))
    {
      case 0: // type
        symbol->setSymbolType(q.value(1).toString());
        break;
      case 1: // name
        symbol->setName(q.value(1).toString());
        break;
      case 2: // table
        symbol->setTable(q.value(1).toString());
        break;
      default:
        break;
    }
  }

  return rc;
}

int Database::deleteSymbol (Bars *bd)
{
  if (! getSymbol(bd))
  {
    qDebug() << "Database::deleteSymbol: invalid SYMBOL";
    return 0;
  }

  // drop quote table
  QSqlQuery q(_db);

  QString s = "DROP TABLE " + bd->table();
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::deleteSymbol:" + q.lastError().text();
    return 0;
  }

  // remove index records for symbol
  s = "DELETE FROM symbolIndex WHERE name='" + bd->symbol() + "'";
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::deleteSymbol:" + q.lastError().text();
    return 0;
  }

  return 1;
}

QList<Bars> Database::search (QString search)
{
  QList<Bars> symbols;
  if (! init())
    return symbols;

  QSqlQuery q(_db);

  // first find matching names
  QString s = "SELECT DISTINCT name FROM symbolIndex";

  if (search.contains("%"))
    s.append(" WHERE name LIKE '" + search + "'");
  else
    s.append(" WHERE name='" + search + "'");

  s.append(" ORDER BY name ASC");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::search:" + q.lastError().text();
    return symbols;
  }

  QStringList names;
  while (q.next())
    names << q.value(0).toString();

  // load names with data
  int loop = 0;
  for (; loop < names.size(); loop++)
  {
    Bars sym;
    sym.setSymbol(names.at(loop));
    
    if (! getSymbol(&sym))
      continue;
    
    symbols << sym;
  }

  return symbols;
}

int Database::setName (Bars *symbol)
{
  QString name = symbol->name();
  if (name.isEmpty())
  {
    qDebug() << "Database::getSymbol: invalid NAME";
    return 0;
  }

  QSqlQuery q(_db);

  QString s = "UPDATE symbolIndex";
  s.append(" SET value='" + name + "'");
  s.append(" WHERE name='" + symbol->symbol() + "'");
  s.append(" AND key='name'");
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::setName:" + q.lastError().text();
    return 0;
  }

  return 1;
}

QDateTime Database::getMaxDate (Bars *symbol)
{
  QSqlQuery q(_db);

  QDateTime dt;
  QString s = "SELECT max(date) FROM " + symbol->table();
  q.exec(s);
  if (q.lastError().isValid())
  {
    qDebug() << "Database::getMaxDate:" + q.lastError().text();
    qDebug() << s;
    return dt;
  }

  if (q.next())
    dt = QDateTime::fromString(q.value(0).toString(), "yyyyMMddHHmmss");

  return dt;
}

Entity* Database::querySettings ()
{
  Entity *pEntity = new Entity;
  return pEntity;
}

// do not remove
Q_EXPORT_PLUGIN2(database, Database)
