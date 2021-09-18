#include "stock.h"

Stock::Stock(QVariantMap &map)
{

    mId = map["marketID"].toInt();
    mName = map["shortname"].toString();
    mIdentifier = map["identifier"].toString();
}


int Stock::getMarketId(){
    return mId;
}

QString Stock::getName(){
    return mName;
}

QString Stock::getIdentifier() {
    return mIdentifier;
}
