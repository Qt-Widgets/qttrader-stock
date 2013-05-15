/*
 *  Qtstalker stock charter
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

#include "NetfondsHistoryDownload.h"
#include "PluginFactory.h"
#include "bar/Bars.h"
#include "bar/BarType.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtNetwork>

NetfondsHistoryDownload::NetfondsHistoryDownload (QObject *p) : QObject (p)
{
  _stop = false;
}

NetfondsHistoryDownload::~NetfondsHistoryDownload()
{
    _stop = true;
}

void NetfondsHistoryDownload::download (QStringList symbolFiles, QDateTime sd, QDateTime ed)
{
  _stop = false;

  QNetworkAccessManager manager;

  int loop = 0;
  for (; loop < symbolFiles.size(); loop++)
  {
    if (_stop == true)
      break;
    
    QFile f(symbolFiles.at(loop));
    if (! f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      QStringList mess;
      mess << tr("Error opening file") << symbolFiles.at(loop) << tr("skipped");
      emit signalMessage(mess.join(" "));
      continue;
    }

    while (! f.atEnd() && _stop == false)
    {
      QString symbol = f.readLine();
      symbol = symbol.trimmed();
      if (symbol.isEmpty())
        continue;

      // get name
      QString name;
//      downloadName(symbol, name);

      name = symbol;

      // get the url
      QString url;
      getUrl(sd, ed, symbol, url);
      QStringList mess;
      mess << url;
      mess << tr("Downloading") << symbol;
      emit signalMessage(mess.join(" "));

      // download the data
      QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
      QEventLoop e;
      QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &e, SLOT(quit()));
      e.exec();

      // parse the data and save quotes
      QByteArray ba = reply->readAll();
      parseHistory(ba, symbol, name);
    }

    f.close();
  }

  // send status message
//  if (_stop)
//    emit signalMessage(tr("Download stopped"));
//  else
//    emit signalMessage(tr("Download complete"));
}

void NetfondsHistoryDownload::getUrl (QDateTime sd, QDateTime ed, QString symbol, QString &url)
{
  url="http://www.netfonds.se/quotes/paperhistory.php?paper=";
  url.append(symbol);
  url.append("&csv_format=csv");
}


void NetfondsHistoryDownload::parseHistory (QByteArray &ba, QString &symbol, QString &name)
{
  //quote_date,paper,exch,open,high,low,close,volume,value
  //20121101,OMXS30,Stockholm,1050.03,1064.08,1047.28,1062.51,0,0
  QString ts(ba);
  QStringList ll = ts.split('\n');

  Bars sym;
  sym.setExchange(QString("NETFONDS"));
  sym.setTicker(symbol);
  sym.setName(name);
  sym.setSymbolType(QString("Stock"));

  BarType bt;
  int line = 0;
  int loop = 1; // skip past first line
  for (; loop < ll.size()-2; loop++)
  {
    line++;
    ts = ll[loop].remove('"');
    ts = ts.trimmed();

    if (! ts.length()){

        QStringList mess;
        mess << symbol << tr("Error!!");
        emit signalMessage(mess.join(" "));
        return;
    }

    QStringList l = ts.split(",");
    if (l.size() != 9)
    {
      QStringList mess;
      mess << symbol << ":" << tr("invalid # of fields, record skipped");
      emit signalMessage(mess.join(" "));
      return;
    }

    CBar *bar = new CBar;

    QDateTime dt = QDateTime::fromString(l.at(0).trimmed(), "yyyyMMdd");
    if (! dt.isValid())
    {
      delete bar;
      QStringList mess;
      mess << symbol << ":" << tr("invalid date") << l.at(0);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->setDate(dt);

    // verify open
    int pos = 3;
    bool ok;
    double t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid open") << l.at(pos);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->set(bt.indexToString(BarType::_OPEN), t);

    // verify high
    pos = 4;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid high") << l.at(pos);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->set(bt.indexToString(BarType::_HIGH), t);

    // verify low
    pos = 5;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid low") << l.at(pos);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->set(bt.indexToString(BarType::_LOW), t);

    // verify close
    pos = 6;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid close") << l.at(pos);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->set(bt.indexToString(BarType::_CLOSE), t);

    // verify volume
    pos = 7;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid volume") << l.at(pos);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->set(bt.indexToString(BarType::_VOLUME), t);

    sym.setBar(sym.bars(), bar);
  }

  IDBPlugin *plug = dynamic_cast<IDBPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(QString("Database")));
  if (! plug)
  {
    QStringList mess;
    mess << tr("Error, Database plugin missing");
    emit signalMessage(mess.join(" "));
    return;
  }

  if (! plug->init())
    return;

  if (! plug->setBars(&sym))
    return;
}


int NetfondsHistoryDownload::downloadName (QString symbol, QString &name)
{
  QString url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
  url.append(symbol);
  url.append("&f=n");
  url.append("&e=.csv");

  QNetworkAccessManager manager;
  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
  QEventLoop e;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &e, SLOT(quit()));
  e.exec();

  // parse the data and save quotes
  QByteArray ba = reply->readAll();
  QString s(ba);
  s = s.remove('"');
  s = s.remove(',');
  s = s.trimmed();
  if (s.isEmpty())
    return 0;

  name = s;

  return 1;
}

void NetfondsHistoryDownload::stop ()
{
  _stop = true;
  emit signalMessage(tr("Stopping download..."));
}
