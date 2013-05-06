#include "trade.h"
#include <QVariantMap>
#include <QString>

Trade::Trade(QVariantMap &map)
{
    accno = map["accno"].toString();            //The Nordnet account number.
    orderID = map["orderID"].toInt();          //The Nordnet orderid of the traded order.
    //instrumentID
    marketID = map["marketID"].toInt();         //The Nordnet market identifier.
    identifier = map["identifier"].toString();       //The exchange specific identifier (feedcode, orderbookid or similar)
    volume = map["volume"].toFloat();           //The volume of the trade
    //price node
    curr = map["curr"].toString();             //Currency.
    value = map["value"].toFloat();            //The price.
    side = map["side"].toString();             //"BUY" or "SELL"
    tradeID = map["tradeID"].toString();          //The exchange tradid
    counterparty = map["counterparty"].toString();     //The trade counterparty.
    tradetime = QDateTime::fromMSecsSinceEpoch(map["tradetime"].toULongLong());        //The exchange time of trade.
}
