#ifndef NORDNETSTREAM_H
#define NORDNETSTREAM_H

#include <QString>
#include <QObject>
#include <QSslSocket>
#include <QAbstractSocket>


class NordnetStream : public QObject
{
    Q_OBJECT
signals:
    void data(QByteArray);
    void disconnected();

public:
    NordnetStream();
    void socketConnect(QByteArray data, QString url, int port);
    void startSubscription(QByteArray data);
//    void connect(QString data);

public slots:
    void readData();
    void onError(QAbstractSocket::SocketError);
    void onSslErrors(const QList<QSslError>&);
    void onDisconnected();

private:
    QSslSocket* m_pSslSocket;
};

#endif // NORDNETSTREAM_H
