#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QVariantMap>

class Index
{
public:
    Index(QVariantMap &map);

    QString getName() { return mName; }
    QString getSource() { return mSource; }
    QString getId() { return mId; }
    QString getType() { return mtype; }
    QString getCountry() { return  mCountry; }

private:
    QString mName;
    QString mSource;
    QString mId;
    QString mtype;
    QString mCountry;
};

#endif // INDEX_H
