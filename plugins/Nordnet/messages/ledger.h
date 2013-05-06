#ifndef LEDGERS_H
#define LEDGERS_H

#include <QVariantMap>
#include <QString>

class Ledger
{
public:
    Ledger(QVariantMap &map);
//private:
    QString currency;       //Currency for this currency account
    float accountSum;       //The amount of money in this currency.
    float accountSumAcc;    //The amount of money including accrued intersest.
    float accIntCred;       //Accrued interest credit.
    float accIntDeb;        //Accrued interest debit.
};

#endif // LEDGERS_H
