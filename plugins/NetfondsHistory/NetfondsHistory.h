/*
 *  Netfonds History Downloader plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#ifndef PLUGIN_NETFONDS_HISTORY_H
#define PLUGIN_NETFONDS_HISTORY_H

#include <QObject>
#include "GUIWidget.h"
#include "QtTraderPlugin.h"

class NetfondsHistory : public QObject, public IGUIPlugin
{
  Q_OBJECT
    Q_INTERFACES(IGUIPlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:
    QString pluginName() { return QString("NetfondsHistory Downloader plugin");}
    QString pluginVersion() { return QString("0.1");}
    Entity* querySettings();
    Widget* create();
};

#endif
