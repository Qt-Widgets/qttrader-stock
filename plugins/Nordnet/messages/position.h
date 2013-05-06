#ifndef POSITION_H
#define POSITION_H

#include <QString>
#include <QVariantMap>

class Position
{
public:
    Position(QVariantMap &map);


    float getAcqPrice() { return acqPrice; }     //Acquire price.
    float getAcqPriceAcc() { return acqPriceAcc; }  //Acquire price in the account currency.
    float getPawnPercent() { return pawnPercent; } //Max percent the user can borrow using this position as security.
    float getQuantity() { return qty; }          //Quantity
    float getMarketValue() { return marketValue; }      //Market value.
    float getMarketValueAcc() { return marketValueAcc; }   //Market value in the account currency.
    //instrumentID 	node
    int getMainMarketId() { return mainMarketId; }        //Nordnet market id
    QString getIdentifier() { return identifier; }      //Instrument identifier
    QString getType() { return type; }            //Instrument type
    QString getCurrency() { return currency; }        //Currency for the instrument on the main market
    float getMainMarketPrice() { return mainMarketPrice; }   //The last price of the instrument on the main market

private:
    float acqPrice;     //Acquire price.
    float acqPriceAcc;  //Acquire price in the account currency.
    float pawnPercent;  //Max percent the user can borrow using this position as security.
    float qty;          //Quantity
    float marketValue;      //Market value.
    float marketValueAcc;   //Market value in the account currency.
    //instrumentID 	node
    int mainMarketId;        //Nordnet market id
    QString identifier;      //Instrument identifier
    QString type;            //Instrument type
    QString currency;        //Currency for the instrument on the main market
    float mainMarketPrice;   //The last price of the instrument on the main market
};

#endif // POSITION_H
