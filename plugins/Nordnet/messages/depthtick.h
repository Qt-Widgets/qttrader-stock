#ifndef DEPTHTICK_H
#define DEPTHTICK_H

#include "tick.h"

class DepthTick : public Tick
{
public:
    DepthTick(QVariantMap &map);
private:
    float bidPrice1;
    int bidVolume1;
    float bidPrice2;
    int bidVolume2;
    float bidPrice3;
    int bidVolume3;
    float bidPrice4;
    int bidVolume4;
    float bidPrice5;
    int bidVolume5;


    float askPrice1;
    int askVolume1;
    float askPrice2;
    int askVolume2;
    float askPrice3;
    int askVolume3;
    float askPrice4;
    int askVolume4;
    float askPrice5;
    int askVolume5;
};

#endif // DEPTHTICK_H
