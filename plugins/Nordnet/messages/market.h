#ifndef MARKET_H
#define MARKET_H

#include <QString>
#include <QVariantMap>

class Market
{
public:
    Market(QVariantMap&);

    QString getName();
    QString getId();
    QString getCountry();

private:
    QString mName;
    QString mId;
    QString mCountry;
};

#endif // MARKET_H
