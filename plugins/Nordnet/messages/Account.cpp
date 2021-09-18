#include "Account.h"

Account::Account(QVariantMap &map)
{
    id = map["id"].toString();
    defaultAccount  = map["default"].toBool();
}
