#include "tick.h"

Tick::Tick(QVariantMap &map)
{

    instrument = map["i"].toString();
    marketPlace = map["m"].toString();
    TickTimeStamp = QDateTime::fromString(map["tick_timestamp"].toString(), QString("yyyy-MM-dd hh:mm:ss"));
}
