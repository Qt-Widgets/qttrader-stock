#include "pricetick.h"
#include <QVariant>
//#include <QtGui>

PriceTick::PriceTick(QVariantMap &map) : Tick (map)
{
//     qDebug() <<"new PriceTick";
     askPrice = map["ask"].toString();
     askVolume = map["ask_volume"].toString();
     bidPrice = map["bid"].toString();
     bidVolume = map["bid_volume"].toString();

     close = map["close"].toString();
     high = map["high"].toString();
     low = map["low"].toString();
     open = map["open"].toString();

     lastPrice = map["last"].toString();
     lastVolume = map["last_volume"].toString();

     lotSize = map["lot_size"].toString();
     turnOver = map["turnover"].toString();
     turnOverVolume = map["turnover_volume"].toString();
     tradeTimeStamp = QDateTime::fromString(map["trade_timestamp"].toString(), QString("yyyy-MM-dd hh:mm:ss"));
}
