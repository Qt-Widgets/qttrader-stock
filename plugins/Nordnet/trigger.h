#ifndef TRIGGER_H
#define TRIGGER_H

#include <QString>
#include "order.h"

class Trigger
{
public:
    Trigger();

    QString triggerMarketID;
    int triggerIdentifier;
    QString tiggerLevel;
    QString triggerSide;

    int orderIdentifier;
    QString orderPrice;
    QString orderVolume;
    QString orderSide;


};

#endif // TRIGGER_H
