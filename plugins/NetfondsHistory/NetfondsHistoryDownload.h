/*
 *  Qtstalker stock charter
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

#ifndef NETFONDS_HISTORY_DOWNLOAD_HPP
#define NETFONDS_HISTORY_DOWNLOAD_HPP

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QByteArray>

class NetfondsHistoryDownload : public QObject
{
  Q_OBJECT
  
  signals:
    void signalMessage (QString);

  public:
    ~NetfondsHistoryDownload();
    NetfondsHistoryDownload(QObject *);
    void download(QStringList symbolFiles);
    void getUrl(QString symbol, QString &url);
    void parseHistory(QByteArray &ba, QString &symbol, QString &name);

public slots:
  void stop();

  private:
    bool _stop;
};

#endif
