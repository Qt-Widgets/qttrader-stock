/*
 *  Qttrader - Nordnet plugin
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#include "GUIWidget.h"
#include "DateRange.h"
#include "NordnetModel.h"

#include "ledger.h"
#include "market.h"
#include "stock.h"
#include "trigger.h"

#include "ui_logindialog.h"
#include <QtUiTools>
#include <QtGui>
#include <QSettings>
#include <QLineEdit>
#include "Global.h"
#include "pricetick.h"

GUIWidget::GUIWidget (NordnetModel *pModel) : m_pRequester(0)
{
  m_pModel = pModel;
  QObject::connect(m_pModel, SIGNAL(answer(QString)), this, SLOT(answer(QString)));
  createGUI();
}

GUIWidget::~GUIWidget ()
{
    qWarning() << "GUI deleted";
}

void GUIWidget::createGUI ()
{
  connect(m_pModel, SIGNAL(loginDone()), this, SLOT(onLogin()));
  connect(m_pModel, SIGNAL(accountsDone()), this, SLOT(onAccounts()));
  connect(m_pModel, SIGNAL(marketsDone()), this, SLOT(onMarkets()));
  connect(m_pModel, SIGNAL(stocksDone()), this, SLOT(onStocks()));
  connect(m_pModel, SIGNAL(orderModelUpdated()), this, SLOT(onOrderModelUpdated()));
  connect(m_pModel, SIGNAL(tradeModelUpdated()), this, SLOT(onTradeModelUpdated()));
  connect(m_pModel, SIGNAL(positionModelUpdated()), this, SLOT(onPositionModelUpdated()));

  QUiLoader loader;
  QFile file(":/forms/tabwidget");
  file.open(QFile::ReadOnly);
  QWidget *pWidget = loader.load(&file, this);
  file.close();

  QPushButton* pRemovebutton = qFindChild<QPushButton*>(pWidget, "removeSubscribtion");
  connect(pRemovebutton, SIGNAL(clicked()), m_pModel, SLOT(remove()));

  QPushButton* pPushbutton = qFindChild<QPushButton*>(pWidget, "logout");
  connect(pPushbutton, SIGNAL(clicked()), m_pModel, SLOT(logout()));

  QComboBox* pMarketsList = qFindChild<QComboBox*>(pWidget, "marketsList");
  connect(pMarketsList, SIGNAL(activated(int)), this, SLOT(stocksRequested()));
  connect(pMarketsList, SIGNAL(activated(int)), m_pModel, SLOT(getStocks(int)));

  QComboBox* pTriggerMarketsList = qFindChild<QComboBox*>(pWidget, "triggerMarketsList");
  connect(pTriggerMarketsList, SIGNAL(activated(int)), this, SLOT(stocksRequested()));
  connect(pTriggerMarketsList, SIGNAL(activated(int)), m_pModel, SLOT(getStocks(int)));

  QComboBox* pOrderMarketsList = qFindChild<QComboBox*>(pWidget, "orderMarketsList");
  connect(pOrderMarketsList, SIGNAL(activated(int)), this, SLOT(stocksRequested()));
  connect(pOrderMarketsList, SIGNAL(activated(int)), m_pModel, SLOT(getStocks(int)));

  QComboBox* pStocksList = qFindChild<QComboBox*>(pWidget, "stocksList");
  connect(pStocksList, SIGNAL(activated(int)), m_pModel, SLOT(getStockData(int)));
//  connect(pStocksList, SIGNAL(activated(int)), m_pModel, SLOT(getChartForStock(int)));

  QPushButton* psub = qFindChild<QPushButton*>(pWidget, "startSub");
  connect(psub, SIGNAL(clicked()), m_pModel, SLOT(getIndex()));

  QPushButton* pSell = qFindChild<QPushButton*>(pWidget, "sellButton");
  connect(pSell, SIGNAL(clicked()), this, SLOT(placeOrder()));

  QPushButton* pBuy = qFindChild<QPushButton*>(pWidget, "buyButton");
  connect(pBuy, SIGNAL(clicked()), this, SLOT(placeOrder()));


  QComboBox* pOrderSide = qFindChild<QComboBox*>(this, "orderSide");

  pOrderSide->addItem(QString("Buy"));
  pOrderSide->addItem(QString("Sell"));

  QComboBox* pTriggerSide = qFindChild<QComboBox*>(this, "triggerSide");

  pTriggerSide->addItem(QString("Buy Over"));
  pTriggerSide->addItem(QString("Buy Under"));
  pTriggerSide->addItem(QString("Sell Over"));
  pTriggerSide->addItem(QString("Sell Under"));
  pTriggerSide->addItem(QString("Last trade Over"));
  pTriggerSide->addItem(QString("Last trade Under"));


  QPushButton* pTrigger = qFindChild<QPushButton*>(pWidget, "triggerButton");
  connect(pTrigger, SIGNAL(clicked()), this, SLOT(addTrigger()));

  connect(this, SIGNAL(placeSellOrder(int,QString,QString)), m_pModel, SLOT(placeSellOrder(int,QString,QString)));
  connect(this, SIGNAL(placeBuyOrder(int,QString,QString)), m_pModel, SLOT(placeBuyOrder(int,QString,QString)));

  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->setSpacing(2);
  vbox->setMargin(5);
  setLayout(vbox);

  vbox->addWidget(pWidget);

  if(!m_pModel->isAutenticated()){
      qWarning() << "is not authenticated";
      showLoginDialog();
  }else{
     qWarning() << "is Autenticated";
     onLogin();
  }
}

void GUIWidget::showLoginDialog(){
    QUiLoader loader;
    QFile file(":/forms/login");
    file.open(QFile::ReadOnly);
    QWidget *pWidget = loader.load(&file, this);
    file.close();
    m_username = qFindChild<QLineEdit*>(pWidget, "username");
    m_password = qFindChild<QLineEdit*>(pWidget, "password");
    QDialogButtonBox* pbuttonBox = qFindChild<QDialogButtonBox*>(pWidget, "buttonBox");
    connect(pbuttonBox, SIGNAL(accepted()), this, SLOT(login()));
    pWidget->show();
}

void GUIWidget::updateGUI ()
{
  QCoreApplication::processEvents();
}

void GUIWidget::answer(QString string){
// _log->append(string);
    qWarning() << string;
}

void GUIWidget::login(){
  qWarning() << "LOGIN";
  m_pModel->login(m_username->text(), m_password->text());
}

void GUIWidget::onLogin()
{
    m_pModel->getAccounts();
    m_pModel->getMarkets();
}

void GUIWidget::onAccounts()
{
    QString name = m_pModel->mAccounts.first()->getId();
    QComboBox* pAccounts = qFindChild<QComboBox*>(this, "comboBox");
    pAccounts->addItem(name);
    if(!m_pModel->mLedgers.isEmpty()){
        QString sum = QString::number(m_pModel->mLedgers.first()->accountSum, 'f', 2);
        QLabel* pSum = qFindChild<QLabel*>(this, "balans");
        pSum->setText(sum);
    }
}


void GUIWidget::onMarkets()
{
    QComboBox* pMarkets = qFindChild<QComboBox*>(this, "marketsList");
    QComboBox* pMarkets2 = qFindChild<QComboBox*>(this, "triggerMarketsList");
    QComboBox* pMarkets3 = qFindChild<QComboBox*>(this, "orderMarketsList");
    pMarkets->clear();
    for(int i = 0; i < m_pModel->mMarkets.size(); i++){
        pMarkets->addItem(m_pModel->mMarkets.at(i)->getName());
        pMarkets2->addItem(m_pModel->mMarkets.at(i)->getName());
        pMarkets3->addItem(m_pModel->mMarkets.at(i)->getName());
    }
}


void GUIWidget::onStocks()
{
    QComboBox* pStocks = qFindChild<QComboBox*>(this, "stocksList");
    QComboBox* pTriggerStocks = qFindChild<QComboBox*>(this, "triggerStocksList");
    QComboBox* pOrderStocks = qFindChild<QComboBox*>(this, "orderStocksList");

    QComboBox* pMarkets = qFindChild<QComboBox*>(this, "marketsList");
    QComboBox* pMarkets2 = qFindChild<QComboBox*>(this, "triggerMarketsList");
    QComboBox* pMarkets3 = qFindChild<QComboBox*>(this, "orderMarketsList");
    if(m_pRequester == pMarkets){
        pStocks->clear();
    }
    if(m_pRequester == pMarkets2){
        pTriggerStocks->clear();
    }
    if(m_pRequester == pMarkets3){
        pOrderStocks->clear();
    }
    for(int i = 0; i < m_pModel->mStocks.size(); i++){
        if(m_pRequester == pMarkets){
            pStocks->addItem(m_pModel->mStocks.at(i)->getName());
        }
        if(m_pRequester == pMarkets2){
            pTriggerStocks->addItem(m_pModel->mStocks.at(i)->getName());
        }
        if(m_pRequester == pMarkets3){
            pOrderStocks->addItem(m_pModel->mStocks.at(i)->getName());
        }
    }
    QObject::disconnect(m_pModel, SIGNAL(tradeTick()), this, SLOT(onTrade()));
    QObject::connect(m_pModel, SIGNAL(tradeTick()), this, SLOT(onTrade()));
}


void GUIWidget::onTrade()
{
    QLabel* pBid = qFindChild<QLabel*>(this, "bid");
    QLabel* pAsk = qFindChild<QLabel*>(this, "ask");
    QLabel* pLatest = qFindChild<QLabel*>(this, "latest");
    QLabel* pVolume= qFindChild<QLabel*>(this, "volume");

    if(m_pModel->mPriceTicks.size() > 0)
    {
        PriceTick* pPriceTick = m_pModel->mPriceTicks.at(m_pModel->mPriceTicks.size()-1);
        pBid->setText(pPriceTick->bidPrice);
        pAsk->setText(pPriceTick->askPrice);
        pLatest->setText(pPriceTick->lastPrice);
        pVolume->setText(pPriceTick->turnOverVolume);

    }
}

void GUIWidget::placeOrder()
{
    QComboBox* pStocksList = qFindChild<QComboBox*>(this, "stocksList");
    QLineEdit* pSum = qFindChild<QLineEdit*>(this, "sum");
    QLineEdit* pPrice = qFindChild<QLineEdit*>(this, "price");

    QPushButton* pSellButton = qFindChild<QPushButton*>(this, "sellButton");
    QPushButton* pBuyButton = qFindChild<QPushButton*>(this, "buyButton");

    if(QObject::sender() == pSellButton){
        emit placeSellOrder(pStocksList->currentIndex(), pSum->text(), pPrice->text());
    }else if(QObject::sender() == pBuyButton){
        emit placeBuyOrder(pStocksList->currentIndex(), pSum->text(), pPrice->text());
    }else{
//        QWarning() << "ERROR! no button clicks when placing order";
    }

}

void GUIWidget::onOrderModelUpdated()
{
    QTableView* pOrderView = qFindChild<QTableView*>(this, "orderView");
    pOrderView->setModel(m_pModel->getOrderViewModel());
}
void GUIWidget::onTradeModelUpdated()
{
    QTableView* pOrderView = qFindChild<QTableView*>(this, "tradeView");
    pOrderView->setModel(m_pModel->getTradeViewModel());
}
void GUIWidget::onPositionModelUpdated()
{
    QTableView* pOrderView = qFindChild<QTableView*>(this, "positionView");
    pOrderView->setModel(m_pModel->getPositionViewModel());
}

void GUIWidget::addTrigger()
{

    QComboBox* pTriggerMarkets = qFindChild<QComboBox*>(this, "triggerMarketsList");
    QComboBox* pTriggerStocks = qFindChild<QComboBox*>(this, "triggerStocksList");
    QComboBox* pTriggerSide = qFindChild<QComboBox*>(this, "triggerSide");
    QLineEdit* pTriggerPrice = qFindChild<QLineEdit*>(this, "triggerLevel");

    QComboBox* pOrderStocks = qFindChild<QComboBox*>(this, "orderStocksList");

    QLineEdit* pOrderVolume = qFindChild<QLineEdit*>(this, "orderVolume");
    QComboBox* pOrderSide = qFindChild<QComboBox*>(this, "orderSide");
    QLineEdit* pOrderPrice = qFindChild<QLineEdit*>(this, "orderPrice");

    Trigger* pTrigger = new Trigger();

    pTrigger->tiggerLevel = pTriggerPrice->text();
    pTrigger->triggerIdentifier = pTriggerStocks->currentIndex();
    pTrigger->triggerMarketID = pTriggerMarkets->currentIndex();
    pTrigger->triggerSide = pTriggerSide->currentText();

    pTrigger->orderIdentifier = pOrderStocks->currentIndex();
    pTrigger->orderPrice = pOrderPrice->text();
    pTrigger->orderVolume = pOrderVolume->text();
    pTrigger->orderSide = pOrderSide->currentText();

    m_pModel->addTrigger(pTrigger);
}

void GUIWidget::stocksRequested()
{
    m_pRequester = dynamic_cast<QComboBox*>(QObject::sender());
}
