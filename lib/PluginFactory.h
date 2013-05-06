/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#ifndef PLUGIN_FACTORY_H
#define PLUGIN_FACTORY_H

#include <QString>
#include <QMap>
#include "QtTraderPlugin.h"

class PluginFactory
{
  public:

    PluginFactory ();
    QObject* loadPluginFromString(QString);
    QtTraderPlugin * loadPlugin (QString);

    static void* getPluginFactory();

private:
    static void* m_pPluginFactory;
    QMap<QString, QtTraderPlugin*> mLoadedPlugins;
};

#endif
