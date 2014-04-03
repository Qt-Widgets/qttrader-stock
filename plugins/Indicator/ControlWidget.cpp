/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2010 Stefan S. Stratigakos
 *  Copyright (C) 2012 - Mattias Johansson
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

#include "ControlWidget.h"
#include "PluginFactory.h"
#include "bar/BarLength.h"
#include "DateRange.h"

#include <QtDebug>
#include <QSettings>
#include <QLayout>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QInputDialog>

ControlWidget::ControlWidget ()
{
  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->setSpacing(5);
  vbox->setMargin(5);

  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->setSpacing(0);
  hbox->setMargin(0);
  hbox->addLayout(vbox, 1);
  setLayout(hbox);

  // pan scrollbar
  _panScrollbar = new QScrollBar(Qt::Horizontal, 0);
  _panScrollbar->setRange(0, 0);
  _panScrollbar->setValue(0);
  connect(_panScrollbar, SIGNAL(valueChanged(int)), this, SIGNAL(signalScrollBarChanged(int)));
  vbox->addWidget(_panScrollbar);

  // zoom scrollbar
  _zoomScrollbar = new QScrollBar(Qt::Horizontal, 0);
  _zoomScrollbar->setRange(0, 0);
  _zoomScrollbar->setValue(0);
  connect(_zoomScrollbar, SIGNAL(valueChanged(int)), this, SIGNAL(signalRange(int)));
  connect(_zoomScrollbar, SIGNAL(valueChanged(int)), this, SLOT(rangeChanged(int)));
  vbox->addWidget(_zoomScrollbar);
}

int ControlWidget::scrollBarValue ()
{
  return _panScrollbar->value();
}

void ControlWidget::resizePanScrollBar (Plot *p)
{
  int page, max;
  p->panScrollBarSize(page, max);
  _panScrollbar->setRange(0, max);
  _panScrollbar->setPageStep(page);

}

void ControlWidget::setPan (int start, int max, int page)
{
  int value = _panScrollbar->value();
  int change = _panScrollbar->maximum()-max;
  _panScrollbar->setValue(max);
  _panScrollbar->setRange(0, max);
  _panScrollbar->setPageStep(page);
  _panScrollbar->setValue(value-change);
}

void ControlWidget::setZoom (int /*min*/, int max, int page){
  resizeZoom(max);
  _zoomScrollbar->setValue(max);
  _zoomScrollbar->setPageStep(page);
}

void ControlWidget::resizeZoom(int max){
    _zoomScrollbar->setRange(31,max);
}

void ControlWidget::rangeChanged(int i)
{
  mPlotRange = i;
  emit signalRefresh();
}

int ControlWidget::getRange()
{
  return mPlotRange;
}

int ControlWidget::loadSettings (DataBase *db)
{
  Entity *e = settings();
  if (! db->get(e))
  {
    delete e;
    return 0;
  }

  QVariant *tset = e->get(QString("panning"));
  if (tset)
  {
    _panScrollbar->setValue(tset->toInt());
  }
  tset = e->get(QString("range"));
  if (tset)
  {
    _zoomScrollbar->setValue(tset->toInt());
  }

  delete e;
  return 1;
}

int ControlWidget::saveSettings (DataBase *db)
{
  Entity *e = settings();
  int rc = db->set(e);
  delete e;
  return rc;
}

Entity* ControlWidget::settings ()
{
  Entity *e = new Entity;
  e->setName(QString("controlWidget"));
  e->set(QString("panning"), new QVariant(_panScrollbar->value()));
  e->set(QString("range"), new QVariant(_zoomScrollbar->value()));
  return e;
}
