#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QVariantMap>

class Account
{
public:
    Account(QVariantMap &map);

    QString getId() { return id; }
    bool isDefault() { return defaultAccount; }

private:
    QString id;
    bool defaultAccount;
};

#endif // ACCOUNT_H
