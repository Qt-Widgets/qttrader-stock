#ifndef TRADE_H
#define TRADE_H

#include <QVariantMap>
#include <QString>
#include <QDateTime>

class Trade
{
public:
    Trade(QVariantMap &map);


    QString getAccount() { return accno; }      //The Nordnet account number.
    int getOrderID() { return orderID; }        //The Nordnet orderid of the traded order.
    //instrumentID
    int getMarketId() { return marketID; }       //The Nordnet market identifier.
    QString getIdentifier() { return identifier; } //The exchange specific identifier (feedcode, orderbookid or similar)
    float getVolume() { return volume; }      //The volume of the trade
    //price node
    QString getCurrency() { return curr; }           //Currency.
    float getValue() { return value; }            //The price.
    QString getSide() { return side; }           //"BUY" or "SELL"
    QString getTrade() { return tradeID; }        //The exchange tradid
    QString getCounterPart() { return counterparty; }   //The trade counterparty.
    QDateTime getTradeTime() { return tradetime; }    //The exchange time of trade.

private:
    QString accno;      //The Nordnet account number.
    int orderID;        //The Nordnet orderid of the traded order.
    //instrumentID
    int marketID;       //The Nordnet market identifier.
    QString identifier; //The exchange specific identifier (feedcode, orderbookid or similar)
    float volume;       //The volume of the trade
    //price node
    QString curr;           //Currency.
    float value;            //The price.
    QString side;           //"BUY" or "SELL"
    QString tradeID;        //The exchange tradid
    QString counterparty;   //The trade counterparty.
    QDateTime tradetime;    //The exchange time of trade.

};

#endif // TRADE_H
