#include "ledger.h"
#include <QString>

Ledger::Ledger(QVariantMap &map)
{
    currency = map["currency"].toString();       //Currency for this currency account
    accountSum = map["accountSum"].toFloat();       //The amount of money in this currency.
    accountSumAcc = map["accountSumAcc"].toFloat();    //The amount of money including accrued intersest.
    accIntCred = map["accIntCred"].toFloat();       //Accrued interest credit.
    accIntDeb = map["accIntDeb"].toFloat();        //Accrued interest debit.
}
