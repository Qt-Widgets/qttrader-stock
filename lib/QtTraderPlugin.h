/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#ifndef QTTRADERPLUGIN_H
#define QTTRADERPLUGIN_H

#include <QPainter>
#include <qwt_plot.h>
#include <QWidget>
#include <QDialog>
#include "Entity.h"
#include "Marker.h"
#include "Bars.h"
#include "Widget.h"
#include "Curve.h"

class QtTraderPlugin {

public:
    virtual ~QtTraderPlugin() {}
    virtual QString pluginName() = 0;
    virtual QString pluginVersion() = 0;

};

class IMarkerPlugin : public QtTraderPlugin {

public:

    virtual Entity* querySettings() = 0;
    //Callbacks from application
    virtual int onMove(Marker* pMarker, int &status, QPoint point) = 0;
    virtual int onClick(Marker* pMarker, int &status, QPoint point, int button) = 0;

    //request for functionality
    virtual int draw (QPainter *, const QwtScaleMap &, const QwtScaleMap &, const QRectF &, void *) = 0;
    virtual int highLow(Entity* pEntity, int &high, int &low, QwtPlot* pPlot, int start, int end) = 0;
    virtual int create(Marker* pMarker, int &status) = 0;
    virtual int info(Entity *pEntity, QStringList &info) = 0;

    //request for objects
    virtual QDialog* getDialog(QWidget *dialogParent, Entity* settings) = 0;

};
class ICurvePlugin : public QtTraderPlugin {
public:
    virtual int draw (QPainter *, const QwtScaleMap &, const QwtScaleMap &, const QRectF &, void *) = 0;
    virtual int highLow(Curve *curve, double &high, double &low, int start, int end) = 0;
    virtual int info(Curve *curve, QStringList &info, int index) = 0;
    virtual int scalePoint(Curve *curve, QColor &color, double &value, int index)  = 0;
    virtual int fill(Curve *curve, QString key1,QString key2,QString key3,QString key4, QColor color)  = 0;
};

class IIndicatorPlugin : public QtTraderPlugin  {

public:

    virtual Entity* querySettings() = 0;
//    virtual int draw (QPainter*, const QwtScaleMap&, const QwtScaleMap&, const QRect&, void*) = 0;
    virtual QDialog* dialog (QWidget *dialogParent, Entity* settings) = 0;
    virtual QList<Curve*> runIndicator (Entity* settings) = 0;
};

class IGUIPlugin : public QtTraderPlugin {

public:

    virtual Entity* querySettings() = 0;
    virtual Widget* create() = 0;
};

class IDBPlugin : public QtTraderPlugin {

public:

    virtual Entity* querySettings() = 0;
    virtual QList<Bars> search (QString search) = 0;
    virtual int init () = 0;
    virtual int setBars (Bars *symbol) = 0;
    virtual int getBars (Bars *symbol) = 0;
    virtual int newTable (Bars *symbol) = 0;
    virtual int newSymbol (Bars *) = 0;
    virtual int getSymbol (Bars *) = 0;
    virtual int deleteSymbol (Bars *) = 0;
    virtual int setName (Bars *) = 0;
    virtual void transaction() = 0;
    virtual void commit() = 0;
};

Q_DECLARE_INTERFACE(ICurvePlugin, "com.QtTrader.IDBPlugin/0.1")
Q_DECLARE_INTERFACE(IDBPlugin, "com.QtTrader.IDBPlugin/0.1")
Q_DECLARE_INTERFACE(IGUIPlugin, "com.QtTrader.IGUIPlugin/0.1")
Q_DECLARE_INTERFACE(IMarkerPlugin, "com.QtTrader.IMarkerPlugin/0.1")
Q_DECLARE_INTERFACE(IIndicatorPlugin, "com.QtTrader.IIndicatorPlugin/0.1")
Q_DECLARE_INTERFACE(QtTraderPlugin, "com.QtTrader.QtTraderPlugin/0.1")

#endif // QTTRADERPLUGIN_H
