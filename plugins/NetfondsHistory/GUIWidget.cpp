/*
 *  Qtstalker stock charter
 *
 *  Copyright (C) 2001-2007 Stefan S. Stratigakos
 *  Copyright (C) 2013 Mattias Johansson
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


#include "GUIWidget.h"
#include "DateRange.h"
#include "NetfondsHistoryDownload.h"
#include "Global.h"

#include <QtGui>
#include <QSettings>


GUIWidget::GUIWidget ()
{
  createGUI();
  loadSettings();
}

GUIWidget::~GUIWidget ()
{
  saveSettings();
}

void GUIWidget::createGUI ()
{
  _timer = new QTimer(this);
  connect(_timer, SIGNAL(timeout()), this, SLOT(updateGUI()));

  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->setSpacing(2);
  vbox->setMargin(5);
  setLayout(vbox);
  
  QFormLayout *form = new QFormLayout;
  form->setSpacing(2);
  form->setMargin(0);
  vbox->addLayout(form);
  
  // log
  QGroupBox *gbox = new QGroupBox;
  gbox->setTitle(tr("Log"));
  vbox->addWidget(gbox);

  QVBoxLayout *tvbox = new QVBoxLayout;
  gbox->setLayout(tvbox);
  
  _log = new QTextEdit;
  _log->setReadOnly(true);
  tvbox->addWidget(_log);

  // buttonbox
  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Help);
  vbox->addWidget(bb);

  // ok button
  _okButton = bb->addButton(QDialogButtonBox::Ok);
  _okButton->setText(tr("&OK"));
  connect(_okButton, SIGNAL(clicked()), this, SLOT(downloadHistory()));

  // cancel button
  _cancelButton = bb->addButton(QDialogButtonBox::Cancel);
  _cancelButton->setText(tr("&Cancel"));
  _cancelButton->setDefault(true);
  _cancelButton->setFocus();
  _cancelButton->setEnabled(false);

}

void GUIWidget::downloadHistory ()
{
  _log->clear();
  setEnabled(false);
  _okButton->setEnabled(false);
  _cancelButton->setEnabled(true);
  
  _timer->start(100);
  
  NetfondsHistoryDownload function(this);
  connect(&function, SIGNAL(signalMessage(QString)), _log, SLOT(append(const QString &)));
  connect(_cancelButton, SIGNAL(clicked()), &function, SLOT(stop()));
  function.download(QStringList(QString("/home/mattias/Dokument/netfonds.txt")));
  
  _timer->stop();

  setEnabled(true);
  _okButton->setEnabled(true);
  _cancelButton->setEnabled(false);
}

void GUIWidget::updateGUI ()
{
  QCoreApplication::processEvents();
}

void GUIWidget::loadSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
}

void GUIWidget::saveSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
}
