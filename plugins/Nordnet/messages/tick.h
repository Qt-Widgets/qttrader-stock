#ifndef TICK_H
#define TICK_H

#include <QString>
#include <QVariantMap>
#include <QDateTime>

class Tick
{
public:
    Tick(QVariantMap &map);

    QString getInstrument(){ return instrument; }
    QString getMarketPlace(){ return marketPlace; }
    QDateTime getTickTimeStamp() { return TickTimeStamp; }


//private:
    QString instrument;
    QString marketPlace;
    QDateTime TickTimeStamp;


};

#endif // TICK_H
