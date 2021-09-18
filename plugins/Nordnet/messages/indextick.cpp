#include "indextick.h"
#include <QVariantMap>

IndexTick::IndexTick(QVariantMap &map) : Tick(map)
{
    last = map["last"].toFloat();
    high = map["high"].toFloat();
    low = map["low"].toFloat();
    close = map["close"].toFloat();
}
