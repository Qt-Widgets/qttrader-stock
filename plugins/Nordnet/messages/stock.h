#ifndef STOCK_H
#define STOCK_H

#include <QVariantMap>
#include <QString>

class Stock
{
public:
    Stock(QVariantMap &map);

    int getMarketId();
    QString getName();
    QString getIdentifier();

private:
    int mId;
    QString mName;
    QString mIdentifier;
};

#endif // STOCK_H
