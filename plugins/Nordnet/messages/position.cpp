#include "position.h"

Position::Position(QVariantMap &map)
{
    acqPrice = map["acqPrice"].toFloat();     //Acquire price.
    acqPriceAcc = map["acqPriceAcc"].toFloat();  //Acquire price in the account currency.
    pawnPercent = map["pawnPercent"].toFloat();  //Max percent the user can borrow using this position as security.
    qty = map["qty"].toFloat();          //Quantity
    marketValue = map["marketValue"].toFloat();      //Market value.
    marketValueAcc = map["marketValueAcc"].toFloat();   //Market value in the account currency.
    //instrumentID 	node
    QVariantMap instrument = map["instrument"].toMap();
    mainMarketId = instrument["mainMarketId"].toInt();        //Nordnet market id
    identifier = instrument["identifier"].toString();      //Instrument identifier
    type = instrument["type"].toString();            //Instrument type
    currency = instrument["currency"].toString();        //Currency for the instrument on the main market
    mainMarketPrice = instrument["mainMarketPrice"].toFloat();   //The last price of the instrument on the main market
}
