#include "index.h"
#include <QVariantMap>

Index::Index(QVariantMap &map)
{
    mName = map["longname"].toString();
    mSource = map["source"].toString();
    mId = map["id"].toString();
    mtype = map["tyupe"].toString();
    mCountry = map["country"].toString();
}
