#include "depthtick.h"

DepthTick::DepthTick(QVariantMap &map) : Tick(map)
{

    bidPrice1 = map["bid1"].toFloat();
    bidVolume1 = map["bid_volume1"].toInt();
    bidPrice2 = map["bid2"].toFloat();
    bidVolume2 = map["bid_volume2"].toInt();
    bidPrice3 = map["bid3"].toFloat();
    bidVolume3 = map["bid_volume3"].toInt();
    bidPrice4 = map["bid4"].toFloat();
    bidVolume4 = map["bid_volume4"].toInt();
    bidPrice5 = map["bid5"].toFloat();
    bidVolume5 = map["bid_volume5"].toInt();

    askPrice1 = map["ask1"].toFloat();
    askVolume1 = map["ask_volume1"].toInt();
    askPrice2 = map["ask2"].toFloat();
    askVolume2 = map["ask_volume2"].toInt();
    askPrice3 = map["ask3"].toFloat();
    askVolume3 = map["ask_volume3"].toInt();
    askPrice4 = map["ask4"].toFloat();
    askVolume4 = map["ask_volume4"].toInt();
    askPrice5 = map["ask5"].toFloat();
    askVolume5 = map["ask_volume5"].toInt();

}
