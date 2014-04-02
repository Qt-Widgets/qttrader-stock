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
#include <QProgressDialog>

NetfondsHistoryDownload::NetfondsHistoryDownload (QObject *p) : QObject (p)
{
}

NetfondsHistoryDownload::~NetfondsHistoryDownload()
{
}

void NetfondsHistoryDownload::download(QStringList symbols)
{
  _stop = FALSE;
  QNetworkAccessManager manager;
  QEventLoop eventLoop;
  QProgressDialog progressDialog("Downloading...", "Cancel", 0, symbols.size());
  connect(&progressDialog, SIGNAL(canceled()), this, SLOT(stop()));
  connect(&progressDialog, SIGNAL(canceled()), &eventLoop, SLOT(quit()));
  progressDialog.setWindowModality(Qt::ApplicationModal);
  progressDialog.show();
  for (int loop = 0; (!_stop && loop < symbols.size()); loop++)
  {
    progressDialog.setValue(loop);
    QString symbol = symbols.at(loop);
    symbol = symbol.trimmed();
    if (symbol.isEmpty())
      continue;

      QString name;

      name = symbol;

      // get the url
      QString url;
      getUrl(symbol, url);
      QStringList mess;
      mess << url;
      mess << tr("Downloading") << symbol;
      emit signalMessage(mess.join(" "));
      mess << "...";
      progressDialog.setLabelText(QString("Downloading ").append(symbol).append("..."));

      // download the data
      QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));

      connect(&progressDialog, SIGNAL(canceled()), &eventLoop, SLOT(quit()));
      QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
      eventLoop.exec();

      // parse the data and save quotes
      QByteArray ba = reply->readAll();
      parseHistory(ba, symbol, name);
  }

}

void NetfondsHistoryDownload::getUrl(QString symbol, QString &url)
{
  url="http://www.netfonds.se/quotes/paperhistory.php?paper=";
  url.append(symbol);
  url.append("&csv_format=csv");
}


void NetfondsHistoryDownload::parseHistory(QByteArray &ba, QString &symbol, QString &name)
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

  if (!plug->init())
    return;

  if (!plug->setBars(&sym))
    return;
}


void NetfondsHistoryDownload::stop()
{
  _stop = TRUE;
}
