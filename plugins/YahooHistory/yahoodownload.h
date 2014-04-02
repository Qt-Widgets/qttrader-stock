#ifndef YAHOODOWNLOAD_H
#define YAHOODOWNLOAD_H

#include <QObject>

class QNetworkReply;

class YahooDownload : public QObject
{
  public:
    YahooDownload();

  public slots:
    void onFinished(QNetworkReply *);
};

#endif // YAHOODOWNLOAD_H
