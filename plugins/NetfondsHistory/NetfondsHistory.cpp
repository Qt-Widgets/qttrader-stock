/*
 *  Netfonds History Downloader plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#include <QtGui>
#include "NetfondsHistoryDownload.h"
#include "GUIWidget.h"
#include "NetfondsHistory.h"

Widget* NetfondsHistory::create()
{
    return new GUIWidget();
}

Entity* NetfondsHistory::querySettings()
{
    return new Entity();
}

// do not remove
Q_EXPORT_PLUGIN2(netfondshistory, NetfondsHistory)
