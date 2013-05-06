#include "order.h"
#include <QDebug>

Order::Order(QVariantMap &map)
{

    exchangeOrderID = map["exchangeOrderID"].toString();
    orderID = map["orderID"].toInt();
    regdate = QDateTime::fromMSecsSinceEpoch(map["modDate"].toULongLong());
    qWarning() << regdate;
    priceCondition = map["priceCondition"].toString();
    volume = map["volume"].toFloat();
    side = map["side"].toString();
    tradedVolume = map["tradedVolume"].toFloat();
    accountNumber = map["accno"].toInt();
    state = map["orderState"].toString();
    actionState = map["actionState"].toString();
//    Instrument instrument
    QVariantMap instrument = map["instrumentID"].toMap();
    identifier = instrument["identifier"].toString();
    marketID = instrument["marketID"].toInt();
//    Validity validity;
//    ActivationCondition activationCondition;
//    Price price;

    QVariantMap price = map["price"].toMap();
    value = price["value"].toInt();
    currency = price["curr"].toString();
}
