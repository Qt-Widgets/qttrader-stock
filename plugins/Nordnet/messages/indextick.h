#ifndef INDEXTICK_H
#define INDEXTICK_H

#include "tick.h"
#include <QVariantMap>

class IndexTick : public Tick
{
public:
    IndexTick(QVariantMap &map);

private:

    float last;
    float high;
    float low;
    float close;
};

#endif // INDEXTICK_H
