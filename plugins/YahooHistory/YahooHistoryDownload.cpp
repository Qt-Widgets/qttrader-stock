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

#include "YahooHistoryDownload.h"
#include "PluginFactory.h"
#include "bar/Bars.h"
#include "bar/BarType.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

YahooHistoryDownload::YahooHistoryDownload (QObject *p) : QObject (p)
{
  _stop = FALSE;
}

YahooHistoryDownload::~YahooHistoryDownload()
{
}

void YahooHistoryDownload::download (QStringList symbols, QDateTime sd, QDateTime ed)
{
  _stop = FALSE;

  QNetworkAccessManager manager;
//  QNetworkAccessManager manager1;

  QProgressDialog progressDialog("Downloading...", "Cancel", 0, symbols.size());
  connect(&progressDialog, SIGNAL(canceled()), &eventLoop, SLOT(quit()));
  connect(&progressDialog, SIGNAL(canceled()), this, SLOT(stop()));
  progressDialog.setWindowModality(Qt::ApplicationModal);
  progressDialog.show();
  for (int loop = 0; (!_stop && loop < symbols.size()); loop++)
  {
    progressDialog.setValue(loop);
    QString symbol = symbols.at(loop);
    symbol = symbol.trimmed();
    if (symbol.isEmpty())
      continue;

    // get name
    QString name;
    downloadName(symbol, name);

    // get the url
    QString url;
    getUrl(sd, ed, symbol, url);

    QStringList mess;
    mess << tr("Downloading") << symbol << "...";
    emit signalMessage(mess.join(" "));
    mess << "...";
    progressDialog.setLabelText(mess.join(" "));

    // download the data
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    // parse the data and save quotes
    QByteArray ba = reply->readAll();
    parseHistory(ba, symbol, name);

    /*Try to download the quotes for today */

//    getLatestUrl(symbol, url);

//    QStringList mess2;
//    mess2 << tr("Downloading today") << symbol << tr(" ") << url;
//    emit signalMessage(mess2.join(" "));

//    // download the data
//    QNetworkReply *reply1 = manager1.get(QNetworkRequest(QUrl(url)));
//    QObject::connect(&manager1, SIGNAL(finished(QNetworkReply *)), &e, SLOT(quit()));
//    e.exec();

//    // parse the data and save quotes
//    QByteArray ba2 = reply1->readAll();
//    parseToday(ba2, symbol, name);

  }

  // send status message
  if (_stop)
    emit signalMessage(tr("Download stopped"));
  else
    emit signalMessage(tr("Download complete"));
}

void YahooHistoryDownload::getUrl (QDateTime sd, QDateTime ed, QString symbol, QString &url)
{
//http://ichart.finance.yahoo.com/table.csv?s=AAPL&d=1&e=22&f=2011&g=d&a=8&b=7&c=1984&ignore=.csv
  url = "http://ichart.finance.yahoo.com/table.csv?s=";
  url.append(symbol);
  url.append("&d=" + QString::number(ed.date().month() - 1));
  url.append("&e=" + ed.date().toString("d"));
  url.append("&f=" + ed.date().toString("yyyy"));
  url.append("&a=" + QString::number(sd.date().month() - 1));
  url.append("&b=" + sd.date().toString("d"));
  url.append("&c=" + sd.date().toString("yyyy"));
  url.append("&ignore=.csv");
}

void YahooHistoryDownload::getLatestUrl(QString symbol, QString &url)
{
    //http://download.finance.yahoo.com/d/quotes.csv?s=%5EOMX&f=sl1d1t1c1ohgv&e=.csv
    url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
    url.append(symbol);
    url.append("&f=sl1d1t1c1ohgv&e=.csv");
}

void YahooHistoryDownload::parseHistory (QByteArray &ba, QString &symbol, QString &name)
{
  QString ts(ba);
  QStringList ll = ts.split('\n');

  Bars sym;
  sym.setExchange(QString("YAHOO"));
  sym.setTicker(symbol);
  sym.setName(name);
  sym.setSymbolType(QString("Stock"));

  BarType bt;
  int line = 0;
  int loop = 1; // skip past first line
  for (; loop < ll.size(); loop++)
  {
    line++;

    ts = ll[loop].remove('"');
    ts = ts.trimmed();

    if (! ts.length())
      continue;

    QStringList l = ts.split(",");
    if (l.size() != 7)
    {
      QStringList mess;
      mess << symbol << ":" << tr("invalid # of fields, record skipped");
      emit signalMessage(mess.join(" "));
      continue;
    }

    CBar *bar = new CBar;

    QDateTime dt = QDateTime::fromString(l.at(0).trimmed(), "yyyy-MM-dd");
    if (! dt.isValid())
    {
      delete bar;
      QStringList mess;
      mess << symbol << ":" << tr("invalid date") << l.at(0);
      emit signalMessage(mess.join(" "));
      continue;
    }
    else
      bar->setDate(dt);

    // verify open
    int pos = 1;
    bool ok;
    double t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid open") << l.at(pos);
      emit signalMessage(mess.join(" "));
      continue;
    }
    else
      bar->set(bt.indexToString(BarType::_OPEN), t);

    // verify high
    pos++;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid high") << l.at(pos);
      emit signalMessage(mess.join(" "));
      continue;
    }
    else
      bar->set(bt.indexToString(BarType::_HIGH), t);

    // verify low
    pos++;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid low") << l.at(pos);
      emit signalMessage(mess.join(" "));
      continue;
    }
    else
      bar->set(bt.indexToString(BarType::_LOW), t);

    // verify close
    pos++;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid close") << l.at(pos);
      emit signalMessage(mess.join(" "));
      continue;
    }
    else
      bar->set(bt.indexToString(BarType::_CLOSE), t);

    // verify volume
    pos++;
    t = l.at(pos).trimmed().toDouble(&ok);
    if (! ok)
    {
      delete bar;
      QStringList mess;
      mess << tr("Error, invalid volume") << l.at(pos);
      emit signalMessage(mess.join(" "));
      continue;
    }
    else
      bar->set(bt.indexToString(BarType::_VOLUME), t);

    sym.setBar(sym.bars(), bar);
  }


  IDBPlugin *plug = dynamic_cast<IDBPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(QString("Database")));
  if (! plug)
  {
    QStringList mess;
    mess << tr("Error, DBSymbol plugin missing");
    emit signalMessage(mess.join(" "));
    return;
  }

  if (!plug->init())
    return;

  if (!plug->setBars(&sym))
    return;
}


void YahooHistoryDownload::parseToday (QByteArray &ba, QString &symbol, QString &name)
{
  QString ts(ba);
  QStringList ll = ts.split('\n');

  Bars sym;
  sym.setExchange(QString("YAHOO"));
  sym.setTicker(symbol);
  sym.setName(name);
  sym.setSymbolType(QString("Stock"));

  BarType bt;

    ts = ll[0].remove('"');
    ts = ts.trimmed();

    if (! ts.length())
      return;

    QStringList l = ts.split(",");
    if (l.size() != 9)
    {
      QStringList mess;
      mess << symbol << ":" << tr("invalid # of fields, record skipped");
      emit signalMessage(mess.join(" "));
      return;
    }

    CBar *bar = new CBar;

    QDateTime dt = QDateTime::fromString(l.at(2).trimmed(), "M/d/yyyy");
    if (! dt.isValid())
    {
      delete bar;
      QStringList mess;
      mess << symbol << ":" << tr("invalid date") << l.at(2);
      emit signalMessage(mess.join(" "));
      return;
    }
    else
      bar->setDate(dt);

    // verify open
    int pos = 5;
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
    pos = 6;
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
    pos = 7;
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
    pos = 1;
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
    pos = 8;
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


  IDBPlugin *plug = dynamic_cast<IDBPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(QString("Database")));
  if (! plug)
  {
    QStringList mess;
    mess << tr("Error, DBSymbol plugin missing");
    emit signalMessage(mess.join(" "));
    return;
  }

  if (!plug->init())
    return;

  if (!plug->setBars(&sym))
    return;
}


int YahooHistoryDownload::downloadName (QString symbol, QString &name)
{
  QString url = "http://download.finance.yahoo.com/d/quotes.csv?s=";
  url.append(symbol);
  url.append("&f=n");
  url.append("&e=.csv");

  QNetworkAccessManager manager;
  manager.get(QNetworkRequest(QUrl(url)));
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onName(QNetworkReply* pReply)));

  // parse the data and save quotes
//  QByteArray ba = reply->readAll();
//  QString s(ba);
//  s = s.remove('"');
//  s = s.remove(',');
//  s = s.trimmed();
//  if (s.isEmpty())
//    return 0;

//  name = s;

  return 1;
}

void YahooHistoryDownload::stop()
{
  _stop = TRUE;
  emit signalMessage(tr("Stopping download..."));
}

bool YahooHistoryDownload::onName(QNetworkReply* pReply)
{
  // parse the data and save quotes
  QByteArray ba = pReply->readAll();
  QString s(ba);
  s = s.remove('"');
  s = s.remove(',');
  s = s.trimmed();
  if (s.isEmpty())
    return false;

  QString name = s;

  return true;
}
