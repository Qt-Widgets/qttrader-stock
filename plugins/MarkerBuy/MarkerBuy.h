/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#ifndef PLUGIN_MARKER_BUY_H
#define PLUGIN_MARKER_BUY_H

#include <QObject>
#include "QtTraderPlugin.h"

class MarkerBuy : public QObject, public IMarkerPlugin
{
  Q_OBJECT
    Q_INTERFACES(IMarkerPlugin)
    Q_INTERFACES(QtTraderPlugin)

  signals:
    void signalMessage (QString);
  
  public:
    QString pluginName() { return QString("Buy Marker");}
    QString pluginVersion() { return QString("0.1");}

    int onMove(Marker* pMarker, int &status, QPoint point);
    int onClick(Marker* pMarker, int &status, QPoint point, int button);

    int info(Entity *pEntity, QStringList &info);
    int highLow(Entity* pEntity, int &high, int &low, QwtPlot* pPlot, int, int);
    int create(Marker* pMarker, int &status);

    Entity* querySettings();
    QDialog* getDialog(QWidget *dialogParent, Entity* settings);

    int draw (QPainter *, const QwtScaleMap &, const QwtScaleMap &, const QRectF &, void *);

};

#endif
