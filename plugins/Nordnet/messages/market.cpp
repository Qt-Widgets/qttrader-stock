#include "market.h"
#include <QVariantMap>
#include <QString>

Market::Market(QVariantMap &map)
{
    mName = map["name"].toString();
    mId = map["id"].toString();
    mCountry = map["country"].toString();
}

QString Market::getName(){
    return mName;
}

QString Market::getId(){
    return mId;
}

QString Market::getCountry(){
    return mCountry;
}
