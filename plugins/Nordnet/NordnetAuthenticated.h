#ifndef NORDNETAUTHENTICATION_H
#define NORDNETAUTHENTICATION_H

#include "Session.h"
#include <QNetworkReply>
#include <string>
//#include <curl/curl.h>


class QByteArray;
class QString;
class QAuthenticator;
class QNetworkAccessManager;
class QSslError;
class QNetworkReply;

class NordnetAuthenticated : public QObject {

    Q_OBJECT

public slots:
    void error(QNetworkReply::NetworkError reply);
    void sslErrors();
    void slotAll();
    void onFinished(QNetworkReply*);

signals:
    void finished(QNetworkReply*);
    void finished(QNetworkReply*, int);

public:
  void login(QString user, QString password);
  void deleteRequest(QString post);
  void putRequest(QString post, QString data);
  void postRequest(QString post, QString data);
  void getRequest(QString get);
  void test();

  virtual ~NordnetAuthenticated();
  NordnetAuthenticated(Session* pSession, const QObject* reciver, const char* slot);
  NordnetAuthenticated(Session* pSession, const QObject* reciver, const char* slot, int marketID);

private:
  QString timestamp_in_ms();
  unsigned char* encrypt(const QString &s, unsigned char *encrypted);
  void parseAccountSummary(const QByteArray &array);
  void parseAccountLedgers(const QByteArray &array);

  QVariant parseReply(QNetworkReply *reply);

  QNetworkRequest authenticateHeader(QNetworkRequest header);
  QNetworkRequest getHeader();

  QNetworkAccessManager * m_pNam;
  QAuthenticator* m_pAuthenticator;

  Session* m_pSession;

  int m_marketID;
};

#endif // NORDNETAUTHENTICATION_H
