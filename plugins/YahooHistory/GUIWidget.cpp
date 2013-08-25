/*
 *  QtTrader stock charter
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
#include "YahooHistoryDownload.h"
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

  QVBoxLayout* vbox = new QVBoxLayout;
  vbox->setSpacing(2);
  vbox->setMargin(5);
  setLayout(vbox);
  
  QHBoxLayout* form = new QHBoxLayout;
  form->setSpacing(2);
  form->setContentsMargins(15,0,0,0);
  vbox->addLayout(form);
  
  m_pAddRowButton = new QPushButton();
  m_pAddRowButton->setText("Add Stock");
  form->addWidget(m_pAddRowButton);
  form->addStretch();

  connect(m_pAddRowButton, SIGNAL(clicked()), this, SLOT(addTableRow()));

  // log
  QGroupBox *gbox = new QGroupBox;
  vbox->addWidget(gbox);

  QHBoxLayout *tvbox = new QHBoxLayout;
  gbox->setLayout(tvbox);
  QSplitter* pSplitView = new QSplitter();
  tvbox->addWidget(pSplitView);

  // Table
  m_pTableWidget = new QTableWidget();
  m_pTableWidget->setColumnCount(5);
  m_pTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_pTableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequseted(QPoint)));

  QStringList stringList;
  stringList.append("Enabled");
  stringList.append("Ticker");
  stringList.append("Start Date");
  stringList.append("End Date");
  stringList.append("Last Update");
  m_pTableWidget->setHorizontalHeaderLabels(stringList);
  m_pTableWidget->resize(10,m_pTableWidget->height());
  m_pTableWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  pSplitView->addWidget(m_pTableWidget);

  _log = new QTextEdit;
  _log->setReadOnly(TRUE);
  _log->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  pSplitView->addWidget(_log);

  // buttonbox
  QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Help);
  vbox->addWidget(bb);

  // ok button
  _okButton = bb->addButton(QDialogButtonBox::Ok);
  _okButton->setText(tr("&OK"));
  connect(_okButton, SIGNAL(clicked()), this, SLOT(downloadHistory()));
}

void GUIWidget::contextMenuRequseted(QPoint point)
{
  QMenu* pMenu = new QMenu();
  QAction* pDeleteAction = new QAction("Delete ticker", this);
  pMenu->addAction(pDeleteAction);
  QAction* selectedItem = pMenu->exec(m_pTableWidget->viewport()->mapToGlobal(point));
  if (selectedItem)
  {
    int row = m_pTableWidget->rowAt(point.y());
    m_pTableWidget->removeRow(row);
  }
}

void GUIWidget::addTableRow()
{
  if(m_pTableWidget){
    int rowNumber = m_pTableWidget->rowCount();
    m_pTableWidget->insertRow(rowNumber);
    QCheckBox* pCheckBox = new QCheckBox();
    pCheckBox->setDisabled(true);
    m_pTableWidget->setCellWidget(rowNumber,0, pCheckBox);
    QDateEdit* pQde1 = new QDateEdit();
    pQde1->setDisabled(true);
    QDateEdit* pQde2 = new QDateEdit();
    pQde2->setDisabled(true);
    pQde2->setDateTime(QDateTime::currentDateTime());
    m_pTableWidget->setCellWidget(rowNumber,2, pQde1);
    m_pTableWidget->setCellWidget(rowNumber,3, pQde2);
  }
}

void GUIWidget::downloadHistory ()
{
  _log->clear();
  setEnabled(FALSE);
  _okButton->setEnabled(FALSE);
  
  DateRange dr;
  QDateTime ed = QDateTime::currentDateTime();
  QDateTime sd = dr.interval(ed, DateRange::_YEAR5);
  
  _timer->start(100);
  
  YahooHistoryDownload function(this);
  connect(&function, SIGNAL(signalMessage(QString)), _log, SLOT(append(const QString &)));
  function.download(getTickers(), sd, ed);
  _timer->stop();

  setEnabled(TRUE);
  _okButton->setEnabled(TRUE);
}

QStringList GUIWidget::getTickers()
{
  QStringList tickers;// = new QStringList();
  for (int i = 0;i < m_pTableWidget->rowCount();i++ )
  {
    QTableWidgetItem* pItem = m_pTableWidget->item(i,1);
    if (pItem){
      tickers.append(pItem->text());
    }
  }
  return tickers;
}

void GUIWidget::updateGUI ()
{
  QCoreApplication::processEvents();
}

void GUIWidget::loadSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
  QStringList tickers = settings.value(QString("Stocks")).toStringList();
  for(int i = 0; i < tickers.size(); i++){
    int row = m_pTableWidget->rowCount();
    addTableRow();
    QTableWidgetItem *pItem = new QTableWidgetItem();
    pItem->setText(tickers.at(i));
    m_pTableWidget->setItem(row, 1, pItem);
  }
}

void GUIWidget::saveSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
  settings.setValue(QString("Stocks"), getTickers());
}
