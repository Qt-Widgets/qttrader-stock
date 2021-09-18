#ifndef ORDER_H
#define ORDER_H

#include <QVariantMap>
#include <QDateTime>
#include <QString>

class Order
{
public:
    Order(QVariantMap &map);

    QString getExchangeOrderId() { return exchangeOrderID; }
    int getOrderId() { return orderID; }
    QDateTime getRegdate() { return regdate; }
    QString getPriceCondition() { return priceCondition; }
    float getVolume() { return volume; }
    QString getSide() { return side; }
    float getTradedVolume() { return tradedVolume; }
    int getAccountNumber() { return accountNumber; }
    QString getState() { return state; }

private:

    QString exchangeOrderID;
    int orderID;
    QDateTime regdate;
    QString priceCondition;
    float volume;
    QString side;
    float tradedVolume;
    int accountNumber;
    QString state;
    QString actionState;
//    Instrument instrument;
    QString identifier;
    int marketID;
//    Validity validity;
//    ActivationCondition activationCondition;
//    Price price;
    int value;
    QString currency;


};

#endif // ORDER_H
