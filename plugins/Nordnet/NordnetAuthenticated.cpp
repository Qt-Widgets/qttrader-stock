/* Copyright 2013 Mattias Johansson
 * All rights reserved
 */

#include "qttrader_defines.h"
#include "NordnetAuthenticated.h"
#include <QVariant>
#include <QMap>
#include <QString>
#include <QDir>
#include <QUrl>
#include <QDateTime>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QSslConfiguration>
#include <QNetworkReply>
#include <QAuthenticator>

#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/evp.h>

#define PUBLIC_KEY_FILE "NEXTAPI_TEST_public.pem"
#define LOGIN_STRING "https://api.test.nordnet.se/next/1/"

using namespace std;

NordnetAuthenticated::NordnetAuthenticated(Session* pSession, const QObject* reciver, const char* slot){
    qWarning() << "Session = " << pSession;
    m_pSession = pSession;
    m_pNam = new QNetworkAccessManager();
    connect(m_pNam, SIGNAL(finished(QNetworkReply*)), this, SIGNAL(finished(QNetworkReply*)));
    connect(this, SIGNAL(finished(QNetworkReply*)), reciver, slot);
}


NordnetAuthenticated::NordnetAuthenticated(Session* pSession, const QObject* reciver, const char* slot, int marketID){
    qWarning() << "Session = " << pSession;
    m_pSession = pSession;
    m_marketID = marketID;
    m_pNam = new QNetworkAccessManager();
    connect(m_pNam, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
    connect(this, SIGNAL(finished(QNetworkReply*, int)), reciver, slot);
}

NordnetAuthenticated::~NordnetAuthenticated(){
    delete m_pNam;
    m_pNam = 0;
    m_pSession = 0;
}


void NordnetAuthenticated::onFinished(QNetworkReply* pQNetworkReply)
{
    emit finished(pQNetworkReply, m_marketID);
}

void NordnetAuthenticated::login(QString user, QString password)
{
   qWarning() << " Start Login";
   QNetworkRequest request;
   QNetworkReply* reply;
   request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

   request.setRawHeader("Accept-Language", "sv");
   request.setRawHeader("Accept", "application/json");
   request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
   request.setUrl(QUrl(QString(LOGIN_STRING).append("login")));


   QByteArray username64 = user.toAscii().toBase64();
   QByteArray password64 = password.toAscii().toBase64();
   QByteArray timestamp64 = timestamp_in_ms().toAscii().toBase64();
   QString toEncrypt = QString(username64).append(":").append(password64).append(":").append(timestamp64);
   unsigned char encryptedStore[2560] = {0};
   unsigned char* test = encrypt(toEncrypt, encryptedStore);
   if(test != NULL){
     QByteArray encrypted = QByteArray((const char*)test,256);
     QByteArray byteArray = QUrl::toPercentEncoding(QString(encrypted.toBase64()));
     QString dataLogin = QString("auth=").append(QString(byteArray)).append("&service=NEXTAPI");
     qWarning() << "URL: " << dataLogin;

     reply = m_pNam->post(request, dataLogin.toAscii());

     connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors()));
     connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
     qWarning() << " End Login";
   }
   qWarning() << "Login failed: no certificate!";
}


void NordnetAuthenticated::test()
{
   QNetworkRequest request = getHeader();
   QNetworkReply* reply;

   request.setUrl(QUrl(QString(LOGIN_STRING)));
   reply = m_pNam->get(request);

   connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors()));
   connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void NordnetAuthenticated::getRequest(QString get)
{
   QNetworkRequest request = authenticateHeader(getHeader());
   QNetworkReply* reply;

   QString logoutString = QString(LOGIN_STRING).append(get);
   request.setUrl(QUrl(logoutString));
   qWarning() << logoutString;
   reply = m_pNam->get(request);

   connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors()));
   connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void NordnetAuthenticated::postRequest(QString post, QString data)
{
   QNetworkRequest request = authenticateHeader(getHeader());
   QNetworkReply* reply;

   QString logoutString = QString(LOGIN_STRING).append(post);
   request.setUrl(QUrl(logoutString));

   reply = m_pNam->post(request, data.toAscii());

   connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors()));
   connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void NordnetAuthenticated::putRequest(QString post, QString data)
{
   QNetworkRequest request = authenticateHeader(getHeader());
   QNetworkReply* reply;

   QString logoutString = QString(LOGIN_STRING).append(post);
   request.setUrl(QUrl(logoutString));

   reply = m_pNam->put(request, data.toAscii());

   connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors()));
   connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}


void NordnetAuthenticated::deleteRequest(QString post)
{
   QNetworkRequest request = authenticateHeader(getHeader());
   QNetworkReply* reply;

   QString logoutString = QString(LOGIN_STRING).append(post);
   request.setUrl(QUrl(logoutString));
   reply = m_pNam->sendCustomRequest(request, "DELETE");

   connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors()));
   connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

QString NordnetAuthenticated::timestamp_in_ms()
{
  QDateTime time;
  return QString(time.toTime_t()).append("000");
}

unsigned char* NordnetAuthenticated::encrypt(const QString &s, unsigned char *encrypted) {
   RSA *public_key;
   FILE *fp;
   OpenSSL_add_all_algorithms();

#ifdef DEBUG
    QString dirString = QDir::currentPath().append("/plugins");
#else
    QString dirString = INSTALL_PLUGIN_DIR;
#endif
   dirString.append("/Nordnet");
   QDir dir = QDir(dirString);
   QString pfileToOpen = dir.absoluteFilePath(PUBLIC_KEY_FILE);
   if(QFileInfo(pfileToOpen).exists()){
     fp = fopen(pfileToOpen.toStdString().c_str(),"r");
     if(fp){
       public_key = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
       fclose(fp);
     } // Todo, proper error handling
     if (!public_key) {
         qFatal("Can't read public key. Recovery unpossible");
         exit(1);
     }
     RSA_public_encrypt(s.length(), reinterpret_cast<unsigned char*>(const_cast<char *>(s.toStdString().c_str())), encrypted, public_key, RSA_PKCS1_PADDING);
     RSA_free(public_key);
     return encrypted;
   }
   return NULL;
 }

void NordnetAuthenticated::error(QNetworkReply::NetworkError reply)
{
    qWarning () << "Found error: " << reply;
//    this->deleteLater();
}

void NordnetAuthenticated::sslErrors()
{
    qWarning () << "Found sslErrors";
//    this->deleteLater();
}

QNetworkRequest NordnetAuthenticated::authenticateHeader(QNetworkRequest header){
    QString concatenated = QString(m_pSession->getSession()) + ":" + QString(m_pSession->getSession());
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    header.setRawHeader("Authorization", headerData.toLocal8Bit());
    return header;
}

QNetworkRequest NordnetAuthenticated::getHeader(){
    QNetworkRequest request;
    QSslConfiguration sslConfiguration = QSslConfiguration::defaultConfiguration();
    request.setSslConfiguration(sslConfiguration);
    request.setRawHeader("Accept-Language", "sv");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    return request;
}

