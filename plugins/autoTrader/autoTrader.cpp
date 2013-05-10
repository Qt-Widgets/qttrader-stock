/*
 *  AutoTrader plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#include <QtGui>

#include "autoTrader.h"
#include "ta_libc.h"
#include "MAType.h"
#include "bar/BarType.h"
#include "curve/CurveLineType.h"
#include "curve/CurveOHLCType.h"
#include "Global.h"
#include "autoTraderDialog.h"

QDialog* autoTrader::dialog (QWidget *dialogParent, Entity *settings)
{
  autoTraderDialog *dialog = new autoTraderDialog(dialogParent);
  dialog->setGUI(settings);
  return dialog;
}


QList<Curve *> autoTrader::runIndicator (Entity *settings)
{
  QList<Curve *> curves;

  if (! g_symbol)
    return curves;
  
  if (!settings)
    return curves;
  
  QVariant *uc = settings->get(QString("upColor"));
  if (! uc)
    return curves;
  
  QVariant *dc = settings->get(QString("downColor"));
  if (! dc)
    return curves;
  
  QVariant *nc = settings->get(QString("neutralColor"));
  if (! nc)
    return curves;
  
  QVariant *label = settings->get(QString("label"));
  if (! label)
    return curves;
  
  QVariant *style = settings->get(QString("style"));
  if (! style)
    return curves;
  
  Curve *OHLC = getOHLC(style->toString(), label->toString(), uc->toString(), dc->toString(), nc->toString());
  if (! OHLC)
    return curves;
  curves << OHLC;
  
  for (int pos = 1; pos < 4; pos++)
  {
    Curve *ma = getMA(settings, pos);
    if (! ma)
      continue;
    
    curves << ma;
  }

  findMACrossings(curves);
  return curves;
}

//Find where Moving avarage lines cross each other to trigger buy/sell signal
void autoTrader::findMACrossings(QList<Curve*> curves){
    if(curves.size() < 2)
    {
        return;
    }
    Curve* ma200 = curves[3];
    Curve* ma50 = curves[2];
    int x = ma50->bars();
    int z = ma200->bars();
    bool positiveClimate;

    //MA200 is valid after 200 time units
    for (int i = 199; i < curves[0]->bars();  i++){
//        qDebug() << "i = " << i;
        Bar* bar200 = ma200->bar(i);
        Bar* bar50 = ma50->bar(i);
        if (i > 199 && bar200 && bar50){
            if (bar200->value() > bar50->value() && positiveClimate) {
                qDebug() << "Sell signal triggered! @ " << i << " Value: 50 =" << bar50->value() << " 200 " <<  bar200->value();
            }else if (bar200->value() < bar50->value() && positiveClimate == false) {
                qDebug() << "Buy signal triggered! @ " << i << " Value 50: " << bar50->value() << " 200 " <<  bar200->value();
            }
        }
        if (bar200 && bar50){
            if (bar200->value() >bar50->value()) {
              positiveClimate = false;
            }else{
              positiveClimate = true;
            }
        }else if (bar200){
            qDebug() << "ERROR! bar50 == null";
        }else if (bar50){
            qDebug() << "ERROR! bar200 == null";
        }else {
            qDebug() << "ERROR! both == null";
        }
    }

// finding top value
//    Bar barNext = ma200->bar(i);
//    Bar bar = ma200->bar(i-1);
//    Bar barPrev = ma200->bar(i-2);

}

Curve *
autoTrader::getOHLC(QString tstyle, QString key, QString tuc, QString tdc, QString tnc)
{
  if (! g_symbol)
    return 0;
  
  Curve *OHLC = new Curve(QString("CurveOHLC"));
  OHLC->setLabel(key);
  
  CurveOHLCType OHLCType;
  OHLC->setStyle(OHLCType.stringToIndex(tstyle));
  
  QColor uc(tuc);
  QColor dc(tdc);
  QColor nc(tnc);
  
  BarType bt;
  OHLC->fill(bt.indexToString(BarType::_OPEN),
             bt.indexToString(BarType::_HIGH),
             bt.indexToString(BarType::_LOW),
             bt.indexToString(BarType::_CLOSE),
             nc);
  
  QList<int> keys = g_symbol->keys();

  for (int pos = 1; pos < keys.size(); pos++)
  {
    Bar *pbar = OHLC->bar(keys.at(pos - 1));
    Bar *bar = OHLC->bar(keys.at(pos));
    if(bar && pbar){
        if (bar->close() > bar->open())
        {
          bar->setColor(uc);
        }
        else if (bar->close() < bar->open())
        {
          bar->setColor(dc);
        }
        else
        {
          bar->setColor(nc);
        }
     }
  }  
  
  return OHLC;
}

Curve *
autoTrader::getMA (Entity *settings, int num)
{
  QString key = "ma" + QString::number(num) + "Input";
  QVariant *input = settings->get(key);
  if (! input)
    return 0;
  
  key = "ma" + QString::number(num) + "Type";
  QVariant *type = settings->get(key);
  if (! type)
    return 0;
  
  key = "ma" + QString::number(num) + "Style";
  QVariant *style = settings->get(key);
  if (! style)
    return 0;
  
  key = "ma" + QString::number(num) + "Width";
  QVariant *width = settings->get(key);
  if (! width)
    return 0;
  
  key = "ma" + QString::number(num) + "Period";
  QVariant *period = settings->get(key);
  if (! period)
    return 0;
  
  key = "ma" + QString::number(num) + "Color";
  QVariant *color = settings->get(key);
  if (! color)
    return 0;
  
  key = "ma" + QString::number(num) + "Label";
  QVariant *label = settings->get(key);
  if (! label)
    return 0;

  MAType mat;
  if (! getMA(input->toString(), label->toString(), mat.stringToIndex(type->toString()), period->toInt()))
    return 0;
  
  Curve *curve = new Curve(QString("CurveLine"));
  curve->setLabel(label->toString());
  CurveLineType clt;
  curve->setStyle(clt.stringToIndex(style->toString()));
  curve->setPen(width->toInt());
  curve->setColor(QColor(color->toString()));
  curve->fill(label->toString(), key, QString(), QString(), QColor());

  return curve;
}

int
autoTrader::getMA (QString inKey, QString outKey, int type, int period)
{
  if (! g_symbol)
    return 0;
  
  TA_RetCode rc = TA_Initialize();
  if (rc != TA_SUCCESS)
    qDebug() << "autoTrader::getMA: error on TA_Initialize";

  QList<int> keys = g_symbol->keys();

  int size = keys.size();
  TA_Real input[size];
  TA_Real out[size];
  TA_Integer outBeg;
  TA_Integer outNb;

  int dpos = 0;
  for (int kpos = 0; kpos < keys.size(); kpos++)
  {
    CBar *bar = g_symbol->bar(keys.at(kpos));
    
    double v;
    if (! bar->get(inKey, v))
      continue;
    
    input[dpos++] = (TA_Real) v;
  }

  rc = TA_MA(0, dpos - 1, &input[0], period, (TA_MAType) type, &outBeg, &outNb, &out[0]);
  if (rc != TA_SUCCESS)
  {
    qDebug() << "autoTrader::getMA: TA-Lib error" << rc;
    return 0;
  }

  int keyLoop = keys.size() - 1;
  int outLoop = outNb - 1;
  while (keyLoop > -1 && outLoop > -1)
  {
    CBar *bar = g_symbol->bar(keys.at(keyLoop));
    bar->set(outKey, out[outLoop]);
    keyLoop--;
    outLoop--;
  }
//TODO this should be here?
//  TA_Shutdown();
  return 1;
}

Entity* autoTrader::querySettings ()
{
  Entity *pEntity = new Entity;
  BarType bt;
  CurveOHLCType ct;
  CurveLineType clt;
  MAType mat;
  
  pEntity->set(QString("plugin"), new QVariant(QString("autoTrader")));
  pEntity->set(QString("type"), new QVariant(QString("indicator")));
  
  pEntity->set(QString("style"), new QVariant(ct.indexToString(CurveOHLCType::_OHLC)));
  pEntity->set(QString("upColor"), new QVariant(QString("green")));
  pEntity->set(QString("downColor"), new QVariant(QString("red")));
  pEntity->set(QString("neutralColor"), new QVariant(QString("blue")));
  pEntity->set(QString("label"), new QVariant(QString("autoTrader")));

  // ma 1
  pEntity->set(QString("ma1Input"), new QVariant(bt.indexToString(BarType::_CLOSE)));
  pEntity->set(QString("ma1Type"), new QVariant(mat.indexToString(MAType::_EMA)));
  pEntity->set(QString("ma1Style"), new QVariant(clt.indexToString(CurveLineType::_SOLID)));
  pEntity->set(QString("ma1Width"), new QVariant(1));
  pEntity->set(QString("ma1Color"), new QVariant(QString("yellow")));
  pEntity->set(QString("ma1Period"), new QVariant(20));
  pEntity->set(QString("ma1Label"), new QVariant(QString("MA1")));

  // ma 2
  pEntity->set(QString("ma2Input"), new QVariant(bt.indexToString(BarType::_CLOSE)));
  pEntity->set(QString("ma2Type"), new QVariant(mat.indexToString(MAType::_EMA)));
  pEntity->set(QString("ma2Style"), new QVariant(clt.indexToString(CurveLineType::_SOLID)));
  pEntity->set(QString("ma2Width"), new QVariant(1));
  pEntity->set(QString("ma2Color"), new QVariant(QString("red")));
  pEntity->set(QString("ma2Period"), new QVariant(50));
  pEntity->set(QString("ma2Label"), new QVariant(QString("MA2")));

  // ma 3
  pEntity->set(QString("ma3Input"), new QVariant(bt.indexToString(BarType::_CLOSE)));
  pEntity->set(QString("ma3Type"), new QVariant(mat.indexToString(MAType::_EMA)));
  pEntity->set(QString("ma3Style"), new QVariant(clt.indexToString(CurveLineType::_SOLID)));
  pEntity->set(QString("ma3Width"), new QVariant(1));
  pEntity->set(QString("ma3Color"), new QVariant(QString("blue")));
  pEntity->set(QString("ma3Period"), new QVariant(200));
  pEntity->set(QString("ma3Label"), new QVariant(QString("MA3")));

  return pEntity;
}

// do not remove
Q_EXPORT_PLUGIN2(autoTrader, autoTrader)
