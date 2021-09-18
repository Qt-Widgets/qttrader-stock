//(c) Mattias Johansson 2013

#ifndef SESSION_H
#define SESSION_H

#include <QVariantMap>
#include <QString>

class Session
{
public:
    Session(QVariantMap &map);

    QString getSession() { return session; }

    QString getPublicHost() { return publicHost; }
    QString getPrivateHost() { return privateHost; }

    int getPublicPort() { return publicPort; }
    int getPrivatePort() { return privatePort; }

    bool getPublicEncrypted() { return publicEncrypted; }
    bool getPrivateEncrypted() { return privateEncrypted; }

private:
    QString session;

    int publicPort;
    QString publicHost;
    bool publicEncrypted;

    int privatePort;
    QString privateHost;
    bool privateEncrypted;
};

#endif // SESSION_H
