/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2007 Stefan S. Stratigakos
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include <QtGui>

#include "Volume.h"
#include "MAType.h"
#include "BarType.h"
#include "CurveLineType.h"
#include "CurveHistogramType.h"
#include "Global.h"
#include "ta_libc.h"
#include "VolumeDialog.h"


QDialog* Volume::dialog(QWidget *dialogParent, Entity* settings)
{
  if (!dialogParent || !settings)
  {
    qDebug() << "Volume::dialog: invalid arguments";
    return 0;
  }
  VolumeDialog *dialog = new VolumeDialog(dialogParent);
  dialog->setGUI(settings);
  return dialog;
}

QList<Curve*> Volume::runIndicator (Entity* settings)
{
  QList<Curve*> curves;
  if (! g_symbol)
    return curves;
  
  QVariant *var = settings->get(QString("upColor"));
  if (! var)
    return curves;
  QColor uc(var->toString());
  
  var = settings->get(QString("downColor"));
  if (! var)
    return curves;
  QColor dc(var->toString());
  
  var = settings->get(QString("neutralColor"));
  if (! var)
    return curves;
  QColor nc(var->toString());
  
  QVariant *label = settings->get(QString("label"));
  if (! label)
    return curves;

  Curve *vol = new Curve(QString("CurveHistogram"));
  vol->setStyle(CurveHistogramType::_BAR);
  vol->setLabel(label->toString());
  
  BarType bt;
  QList<int> keys = g_symbol->keys();
  vol->fill(bt.indexToString(BarType::_VOLUME), QString(), QString(), QString(), nc);
  
  for (int pos = 1; pos < keys.size(); pos++)
  {
    CBar *pbar = g_symbol->bar(keys.at(pos - 1));
    CBar *bar = g_symbol->bar(keys.at(pos));
    Bar *vbar = vol->bar(keys.at(pos));

    double yc = 0;
    if (! pbar->get(bt.indexToString(BarType::_CLOSE), yc))
      continue;

    double c = 0;
    if (! bar->get(bt.indexToString(BarType::_CLOSE), c))
      continue;
    
    if (c > yc)
      vbar->setColor(uc);
    else
    {
      if (c < yc)
        vbar->setColor(dc);
    }
  }  

  curves << vol;
  
  Curve *ma = getMA(settings);
  if (ma)
    curves << ma;
  
  return curves;
}

Curve* Volume::getMA (Entity *settings)
{
  QVariant *type = settings->get(QString("maType"));
  if (! type)
    return 0;
  
  QVariant *period = settings->get(QString("maPeriod"));
  if (! period)
    return 0;
  
  QVariant *var = settings->get(QString("maColor"));
  if (! var)
    return 0;
  QColor color(var->toString());
  
  QVariant *label = settings->get(QString("maLabel"));
  if (! label)
    return 0;
  
  QVariant *style = settings->get(QString("maStyle"));
  if (! style)
    return 0;
  
  QVariant *width = settings->get(QString("maWidth"));
  if (! width)
    return 0;

  BarType bt;
  if (! getMA(bt.indexToString(BarType::_VOLUME), label->toString(), type->toInt(), period->toInt()))
    return 0;
  
  Curve *curve = new Curve(QString("CurveLine"));
  curve->setLabel(label->toString());
  CurveLineType clt;
  curve->setStyle(clt.stringToIndex(style->toString()));
  curve->setPen(width->toInt());
  curve->setColor(color);
  curve->fill(label->toString(), QString(), QString(), QString(), color);

  return curve;
}

int Volume::getMA (QString inKey, QString outKey, int type, int period)
{
  if (! g_symbol)
    return 0;
  
  TA_RetCode rc = TA_Initialize();
  if (rc != TA_SUCCESS)
    qDebug() << "Volume::getMA: error on TA_Initialize";

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
    qDebug() << "Volume::getMA: TA-Lib error" << rc;
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
  
  return 1;
}

Entity* Volume::querySettings ()
{
  Entity *pEntity = new Entity;
  
  // plugin
  pEntity->set(QString("plugin"), new QVariant(QString("Volume")));
  pEntity->set(QString("type"), new QVariant(QString("indicator")));
  pEntity->set(QString("upColor"), new QVariant(QString("green")));
  pEntity->set(QString("downColor"), new QVariant(QString("red")));
  pEntity->set(QString("neutralColor"), new QVariant(QString("blue")));
  pEntity->set(QString("label"), new QVariant(QString("VOL")));

  MAType mat;
  pEntity->set(QString("maType"), new QVariant(mat.indexToString(MAType::_EMA)));

  // style
  CurveLineType clt;
  pEntity->set(QString("maStyle"), new QVariant(clt.indexToString(CurveLineType::_SOLID)));
  
  pEntity->set(QString("maWidth"), new QVariant(1));
  pEntity->set(QString("maColor"), new QVariant(QString("yellow")));
  pEntity->set(QString("maPeriod"), new QVariant(10));
  pEntity->set(QString("maLabel"), new QVariant(QString("MA")));
  
  return pEntity;
}

// do not remove
Q_EXPORT_PLUGIN2(volume, Volume);
