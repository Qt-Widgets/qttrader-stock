/*
 *  Nordnet Downloader plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#ifndef PLUGIN_NORDNET_H
#define PLUGIN_NORDNET_H

#include <QObject>
#include "GUIWidget.h"
#include "QtTraderPlugin.h"
#include "NordnetModel.h"

class Nordnet : public QObject, public IGUIPlugin
{
  Q_OBJECT
    Q_INTERFACES(IGUIPlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:
    Nordnet();
    ~Nordnet();
    QString pluginName() { return QString("Nordnet");}
    QString pluginVersion() { return QString("0.1");}
    Entity* querySettings();
    Widget* create();

private:
    NordnetModel* m_pModel;
};

#endif //PLUGIN_NORDNET_H
