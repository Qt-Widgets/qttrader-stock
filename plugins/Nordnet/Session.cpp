//(c) Mattias Johansson 2013

#include "Session.h"
#include <QVariantMap>
#include <QString>

Session::Session(QVariantMap &map)
{
    session = map["session_key"].toString();

    QVariantMap public_feed = map["public_feed"].toMap();
    QVariantMap private_feed = map["private_feed"].toMap();

    publicPort = public_feed["port"].toInt();
    publicHost  = public_feed["hostname"].toString();
    publicEncrypted = public_feed["encrypted"].toBool();

    privatePort = private_feed["port"].toInt();
    privateHost  = private_feed["hostname"].toString();
    privateEncrypted = private_feed["encrypted"].toBool();
}
