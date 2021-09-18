#ifndef PRICETICK_H
#define PRICETICK_H

#include "tick.h"
#include <QString>
#include <QVariantMap>

class PriceTick : public Tick
{
public:
    PriceTick(QVariantMap &map);
//private:

    QString askPrice;
    QString askVolume;
    QString bidPrice;
    QString bidVolume;

    QString close;
    QString high;
    QString low;
    QString open;

    QString lastPrice;
    QString lastVolume;

    QString lotSize;
    QString turnOver;
    QString turnOverVolume;
    QDateTime tradeTimeStamp;

};

#endif // PRICETICK_H
