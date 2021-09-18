#include "nordnetstream.h"
#include <QSslSocket>
#include <QUrl>

NordnetStream::NordnetStream()
{
}

void NordnetStream::socketConnect(QByteArray data, QString url, int port){
    data.append("\n"); //Always newline
    m_pSslSocket= new QSslSocket();
    m_pSslSocket->connectToHostEncrypted(url, port);

    QObject::connect(m_pSslSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    QObject::connect(m_pSslSocket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(onSslErrors(const QList<QSslError>&)));
    QObject::connect(m_pSslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    QObject::connect(m_pSslSocket, SIGNAL(disconnected ()), this, SLOT(onDisconnected()));

    if(m_pSslSocket->waitForConnected()){
        qWarning() << "Connected!";
        m_pSslSocket->waitForEncrypted();
        m_pSslSocket->write(data.data());
        m_pSslSocket->waitForBytesWritten();
    }
}

void NordnetStream::startSubscription(QByteArray data){
    data.append("\n"); //Always newline
    m_pSslSocket->write(data);
    m_pSslSocket->waitForBytesWritten();
}

void NordnetStream::readData(){
    QByteArray reply = m_pSslSocket->readAll();
    emit data(reply);
    qWarning() << "Data: " << QString(reply.data()) << " object : " << this;
}

void NordnetStream::onError(QAbstractSocket::SocketError){
    qWarning() << "Socket ERRROR";
}

void NordnetStream::onSslErrors(const QList<QSslError>&){
    qWarning() << "SSL Socket ERRROR";
}

void NordnetStream::onDisconnected(){
    qWarning() << "Disconnected";
    emit disconnected();
}
