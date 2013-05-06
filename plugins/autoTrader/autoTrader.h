/*
 *  AutoTrader plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 */

#ifndef PLUGIN_AUTOTRADER_H
#define PLUGIN_AUTOTRADER_H

#include <QObject>

#include "QtTraderPlugin.h"

class autoTrader : public QObject, public IIndicatorPlugin
{
  Q_OBJECT
    Q_INTERFACES(IIndicatorPlugin)
    Q_INTERFACES(QtTraderPlugin)

  public:
    QString pluginName() { return QString("Auto Trader");}
    QString pluginVersion() { return QString("0.1");}

    Entity* querySettings();
    QDialog* dialog (QWidget *dialogParent, Entity* settings);
    QList<Curve*> runIndicator(Entity* settings);
    
  private:
    Curve * getMA (Entity *settings, int num);
    Curve * getOHLC (QString tstyle, QString key, QString tuc, QString tdc, QString tnc);
    int getMA (QString inKey, QString outKey, int type, int period);
    void findMACrossings(QList<Curve *> curves);
};

#endif
