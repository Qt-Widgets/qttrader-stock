/* (c) Mattias Johansson 2013 */

#include "NordnetModel.h"
#include "NordnetAuthenticated.h"
#include "PluginFactory.h"
#include "nordnetstream.h"
#include "Account.h"
#include "Session.h"
#include "order.h"
#include "trade.h"
#include "ledger.h"
#include "position.h"
#include "market.h"
#include "stock.h"
#include "index.h"
#include "pricetick.h"
#include "bar/Bars.h"
#include "bar/BarType.h"

#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QNetworkReply>
#include <qjson/parser.h>
#include <qjson/serializer.h>

using namespace QJson;

NordnetModel::NordnetModel(): m_pSession(0), m_pOrderViewModel(0), m_pTradeViewModel(0), m_pPositionViewModel(0), mPosition(-1), m_pNordnetStream(0)
{

}

void NordnetModel::login (QString userName, QString password)
{
  NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(0, this, SLOT(onLogin(QNetworkReply*)));
  pNordnetAuthenticator->login(userName, password);
}

void NordnetModel::logout()
{
    if(m_pSession){
        NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onLogout(QNetworkReply*)));
        pNordnetAuthenticator->deleteRequest(QString("login/").append(m_pSession->getSession()));
    }
}

void NordnetModel::getAccounts()
{
    if(m_pSession){
        NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onAccount(QNetworkReply*)));
        pNordnetAuthenticator->getRequest(QString("accounts"));
    }else{
        qWarning() << "Error: No session - failed to get accounts!";
    }
}

void NordnetModel::getStockData(int pos){
    QByteArray loginData = getLoginData();
    QByteArray json2 = getSubscriptionString(pos);
    qWarning() << "json string: " << QString(loginData.data());
    qWarning() << "json2 string: " << QString(json2.data());
    if(m_pSession){
//        NordnetStream* pNordnetStream = new NordnetStream();
//        pNordnetStream->socketConnect(loginData, m_pSession->getPublicHost(), m_pSession->getPublicPort());
        if(!m_pNordnetStream)
        {
           m_pNordnetStream = new NordnetStream();
           m_pNordnetStream->socketConnect(loginData, m_pSession->getPublicHost(), m_pSession->getPublicPort());
         }
        QObject::connect(m_pNordnetStream, SIGNAL(data(QByteArray)), this, SLOT(onPriceTick(QByteArray)));
        m_pNordnetStream->startSubscription(json2);
    }else{
        qWarning() << "Error: No session - failed to get stock data!";
    }
}

//void NordnetModel::getIndex()
//{
//    QByteArray loginData = getLoginData();
//    QByteArray json2 = getSubscriptionString();
//    qWarning() << "json string: " << QString(loginData.data());
//    if(m_pSession){
//        NordnetStream* pNordnetStream = new NordnetStream();
//        pNordnetStream->socketConnect(loginData, m_pSession->getPublicHost(), m_pSession->getPublicPort());
//        pNordnetStream->startSubscription(json2);
//    }else{
//        qWarning() << "Error: No session - failed to get indices!";
//    }
//}

void NordnetModel::getMarkets()
{
    if(m_pSession && mMarkets.isEmpty()){
        NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onMarkets(QNetworkReply*)));
        pNordnetAuthenticator->getRequest(QString("lists"));
    }else{
        emit marketsDone();
    }
}

void NordnetModel::getStocks(int pos)
{
    Market* pMarket = mMarkets.at(pos);

    if(mStocksMap.contains(pMarket->getId()))
    {
        mStocks.clear();
        mStocks.append(mStocksMap.value(pMarket->getId()));
        emit stocksDone();
        qWarning() << "Loaded from cache!";
    }
    else if(m_pSession)
    {
        NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onStocks(QNetworkReply*, int)),pMarket->getId().toInt());
        pNordnetAuthenticator->getRequest(QString("lists/").append(pMarket->getId()));
        qWarning() << "Loaded from Nordnet!";
    }
    else
    {
        qWarning() << "Error: No session - failed to get accounts!";
    }
}

void NordnetModel::getChartForStock(int stockPosition)
{
    Stock* pStock = mStocks.at(stockPosition);
    if(m_pSession){
        NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onChart(QNetworkReply*)));
        pNordnetAuthenticator->getRequest(QString("chart_data?marketID=").append(QString::number(pStock->getMarketId())).append("&identifier=").append(pStock->getIdentifier()));
    }else{
        qWarning() << "Error: No session - failed to get accounts!";
    }
}

void NordnetModel::test()
{
    NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(0, this, SLOT(onTest(QNetworkReply*)));
    pNordnetAuthenticator->test();
}


//Callback handlers for replys from server

void NordnetModel::onLogin(QNetworkReply *reply)
{
    qWarning() << "onLogin";
    QVariantMap sessionMap = parseReply(reply).toMap();
    m_pSession = new Session(sessionMap);
    if(m_pSession->getPrivateHost().isEmpty()){
        qDebug() << "Login failed";
        delete m_pSession;
        m_pSession = NULL;
        QMessageBox msgBox;
        msgBox.setWindowTitle("Nordnet");
        msgBox.setText("Login failed.");
        msgBox.exec();
    }
    QObject::sender()->deleteLater();
    emit loginDone();
}

void NordnetModel::onLogout(QNetworkReply* reply){
    parseReply(reply); // to get log printout
    QObject::sender()->deleteLater();
    delete m_pSession;
    m_pSession = 0;
    mAccounts.clear();
}


void NordnetModel::onTest(QNetworkReply *reply){
    parseReply(reply); // to get log printout
    reply->deleteLater();
    QObject::sender()->deleteLater();
}

//Works only for one account
void NordnetModel::onAccount(QNetworkReply *reply){
    mAccounts.clear();
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
    foreach(QVariant record, list) {
        QVariantMap map = record.toMap();
        mAccounts.append(new Account(map));
    }
    QString account = mAccounts.first()->getId();


    NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onAccountSummary(QNetworkReply*)));
    pNordnetAuthenticator->getRequest(QString("accounts/").append(account));
//TODO remove mem leaks!
    pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onLedger(QNetworkReply*)));
    pNordnetAuthenticator->getRequest(QString("accounts/").append(account).append("/ledgers"));

    pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onPosition(QNetworkReply*)));
    pNordnetAuthenticator->getRequest(QString("accounts/").append(account).append("/positions"));

    pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onOrder(QNetworkReply*)));
    pNordnetAuthenticator->getRequest(QString("accounts/").append(account).append("/orders"));

    pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onTrade(QNetworkReply*)));
    pNordnetAuthenticator->getRequest(QString("accounts/").append(account).append("/trades"));

    }else{
        qWarning() << "Error: list is empty";
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
}


void NordnetModel::onAccountSummary(QNetworkReply *reply){
    QVariant variant = parseReply(reply); // to get log printout
    reply->deleteLater();
    QObject::sender()->deleteLater();
//    emit accountsDone();
}

void NordnetModel::onTrade(QNetworkReply *reply){
    mTrades.clear();
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
            mTrades.append(new Trade(map));
        }
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
//    emit accountsDone();
}

void NordnetModel::onOrder(QNetworkReply *reply)
{
    if(m_pOrderViewModel){
        delete m_pOrderViewModel;
    }
    m_pOrderViewModel = new OrderViewModel();
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
//            mOrders.append(new Order(map));
            m_pOrderViewModel->append(new Order(map));
        }
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
    emit orderModelUpdated();
}

void NordnetModel::onLedger(QNetworkReply *reply){

    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
            mLedgers.append(new Ledger(map));
        }
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
    emit accountsDone();
}

void NordnetModel::onPosition(QNetworkReply *reply)
{
    if(m_pPositionViewModel){
        delete m_pPositionViewModel;
    }
    m_pPositionViewModel = new PositionViewModel();
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
//            mPositions.append(new Position(map));
            m_pPositionViewModel->append(new Position(map));
        }
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
//    emit accountsDone();
    emit positionModelUpdated();
}

void NordnetModel::onMarkets(QNetworkReply *reply){
    mMarkets.clear(); //TODO proper memory release!
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
            mMarkets.append(new Market(map));
        }
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
    emit marketsDone();
}

void NordnetModel::onStocks(QNetworkReply *reply, int marketID){
    mStocks.clear(); // TODO proper memory release! When cache is released!
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
            mStocks.append(new Stock(map));
        }
    }
    //Are we sure that all Stocks have the same market?
    mStocksMarketMap.insert(QString::number(mStocks.at(0)->getMarketId()), mStocks);
    mStocksMap.insert(QString::number(marketID), mStocks);
    reply->deleteLater();
    QObject::sender()->deleteLater();
    emit stocksDone();
}

void NordnetModel::onChart(QNetworkReply *reply){
    QVariantList list = parseReply(reply).toList();
//    if(!list.isEmpty()){
//        foreach(QVariant record, list) {
//            QVariantMap map = record.toMap();
//            mStocks.append(new Stock(map));
//        }
//    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
//    emit stocksDone();
    qWarning() << list;
}

void NordnetModel::onIndex(QNetworkReply *reply){
    mIndex.clear(); // TODO proper memory release!
    QVariantList list = parseReply(reply).toList();
    if(!list.isEmpty()){
        foreach(QVariant record, list) {
            QVariantMap map = record.toMap();
            mIndex.append(new Index(map));
        }
    }
    reply->deleteLater();
    QObject::sender()->deleteLater();
    emit stocksDone();
}

void NordnetModel::tickToBars()
{
    Bars sym;
    sym.setExchange(QString("Nordnet"));
    PriceTick* pTick = mPriceTicks.at(mPriceTicks.size()-1);
    sym.setTicker(getTicker(pTick->getMarketPlace(), pTick->getInstrument()));
    sym.setName(getTicker(pTick->getMarketPlace(), pTick->getInstrument()));
    sym.setSymbolType(QString("Stock"));

//    int i = 0;
    BarType bt;
    qDebug() << "mPriceTicks.size: " << mPriceTicks.size();
    for(int i = 0; i < mPriceTicks.size(); i++)
    {
        CBar* pBar = new CBar();
//        pBar->set(bt.indexToString(BarType::_OPEN), mPriceTicks.at(i)->open.toDouble());
//        pBar->set(bt.indexToString(BarType::_CLOSE), mPriceTicks.at(i)->close.toDouble());
//        pBar->set(bt.indexToString(BarType::_HIGH), mPriceTicks.at(i)->high.toDouble());
//        pBar->set(bt.indexToString(BarType::_LOW), mPriceTicks.at(i)->low.toDouble());

        pBar->set(bt.indexToString(BarType::_OPEN), mPriceTicks.at(i)->lastPrice.toDouble());
        pBar->set(bt.indexToString(BarType::_CLOSE), mPriceTicks.at(i)->lastPrice.toDouble());
        pBar->set(bt.indexToString(BarType::_HIGH), mPriceTicks.at(i)->lastPrice.toDouble());
        pBar->set(bt.indexToString(BarType::_LOW), mPriceTicks.at(i)->lastPrice.toDouble());
        pBar->set(bt.indexToString(BarType::_VOLUME), mPriceTicks.at(i)->lastVolume.toDouble());
        pBar->setDate(mPriceTicks.at(i)->getTickTimeStamp());
        sym.setBar(i,pBar);
//        pBar->setDate(mPriceTicks.at(i)->getTickTimeStamp().addDays(y));
//        sym.setBar(y,pBar);
    }

    IDBPlugin *plug = dynamic_cast<IDBPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(QString("Database")));
    if (! plug)
    {
      return;
    }

    if (! plug->init())
      return;

    if (! plug->setBars(&sym))
      return;
}

void NordnetModel::onPriceTick(QByteArray reply){

    QVariantMap map = parseStream(reply).toMap();
    QString cmd = map["cmd"].toString();
    PriceTick* pPriceTick = 0;
    if(cmd == "price"){
//        mPriceTicks.clear();
        QVariantMap priceTick = map["args"].toMap();
        pPriceTick = new PriceTick(priceTick);
        mPriceTicks.append(pPriceTick);
        emit tradeTick();
    }
    tickToBars();
    if (pPriceTick && m_pTriggers.size() > 0){
        for(int i = 0; i < m_pTriggers.size(); i++){
            if(pPriceTick->getInstrument() ==mStocks.at(m_pTriggers.at(i)->triggerIdentifier)->getIdentifier())
            {
                qWarning() << "tick for trigger";
                //TODO fix!!!

                QString triggerSide = m_pTriggers.at(i)->triggerSide;
                QString tiggerLevel = m_pTriggers.at(i)->tiggerLevel;
                if(triggerSide == QString("Last trade Over") && pPriceTick->lastPrice > tiggerLevel){
                    qWarning() << "Trigger";
                }
                if(triggerSide == QString("Last trade Under") && pPriceTick->lastPrice < tiggerLevel){
                    qWarning() << "Trigger";
                }
            }
        }
    }
}

QVariant NordnetModel::parseStream(QByteArray array)
{
    Parser parser;
    bool result;
    QVariant variant = parser.parse(array, &result);

    emit answer(QString(array));

    if (!result) {
        qWarning("An error occured during parsing");
        return variant;
    }
    return variant;
}

QVariant NordnetModel::parseReply(QNetworkReply *reply)
{
    Parser parser;
    bool result;

    QByteArray array = reply->readAll();
    QVariant variant = parser.parse(array, &result);

    emit answer(QString(array));

    if (!result) {
        qWarning("An error occured during parsing");
        return variant;
    }
    return variant;
}

bool NordnetModel::isAutenticated(){
    if(m_pSession){
        return true;
    }
    return false;
}

QByteArray NordnetModel::getLoginData(){
    QVariantMap args;
    QVariantMap data;
    args.insert("service", "NEXTAPI");
    args.insert("session_key", m_pSession->getSession());
    data.insert("args", args);
    data.insert("cmd", "login");

    Serializer serializer;
    return serializer.serialize(data);
}

QByteArray NordnetModel::getSubscriptionString(int pos){
    mPosition = pos;
    Stock* pStock = mStocks.at(pos);
    QVariantMap args2;
    QVariantMap data2;
    args2.insert("t", "price");
    args2.insert("i", pStock->getIdentifier());
    args2.insert("m", pStock->getMarketId());
    data2.insert("args", args2);
    data2.insert("cmd", "subscribe");

    Serializer serializer;
    return serializer.serialize(data2);
}

QByteArray NordnetModel::getUnSubscriptionString()
{
  if(mPosition != -1)
  {
    Stock* pStock = mStocks.at(mPosition);
    QVariantMap args2;
    QVariantMap data2;
    args2.insert("t", "price");
    args2.insert("i", pStock->getIdentifier());
    args2.insert("m", pStock->getMarketId());
    data2.insert("args", args2);
    data2.insert("cmd", "unsubscribe");

    Serializer serializer;
    return serializer.serialize(data2);
  }
  return NULL;
}

void NordnetModel::placeOrder(){

    NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onLogin(QNetworkReply*)));
    pNordnetAuthenticator->postRequest(QString("accounts/").append(mAccounts.first()->getId()).append("/orders"), QString("identifier=101&amp;marketID=11&amp;price=70&amp;side=buy&amp;volume=2&amp;currency=SEK"));
}

void NordnetModel::placeSellOrder(int identifier, QString volume, QString price){

    Stock* pStock = mStocks.at(identifier);
    NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onLogin(QNetworkReply*)));
    QString orderString = QString("identifier=").append(pStock->getIdentifier()).append(QString("&amp;marketID=")).append(QString::number(pStock->getMarketId())).append(QString("&amp;price=")).append(QString(price)).append(QString("&amp;side=sell&amp;volume=")).append(QString(volume)).append(QString("&amp;currency=SEK;"));
    pNordnetAuthenticator->postRequest(QString("accounts/").append(mAccounts.first()->getId()).append("/orders"), orderString);
    qWarning() << orderString;
}

void NordnetModel::placeBuyOrder(int identifier, QString volume, QString price){

    Stock* pStock = mStocks.at(identifier);
    NordnetAuthenticated* pNordnetAuthenticator = new NordnetAuthenticated(m_pSession, this, SLOT(onLogin(QNetworkReply*)));
    QString orderString = QString("identifier=").append(pStock->getIdentifier()).append(QString("&amp;marketID=")).append(QString::number(pStock->getMarketId())).append(QString("&amp;price=")).append(QString(price)).append(QString("&amp;side=buy&amp;volume=")).append(QString(volume)).append(QString("&amp;currency=SEK;"));
    pNordnetAuthenticator->postRequest(QString("accounts/").append(mAccounts.first()->getId()).append("/orders"), orderString);
    qWarning() << orderString;
}

OrderViewModel* NordnetModel::getOrderViewModel()
{
    return m_pOrderViewModel;
}

TradeViewModel* NordnetModel::getTradeViewModel()
{
    return m_pTradeViewModel;
}

PositionViewModel* NordnetModel::getPositionViewModel()
{
    return m_pPositionViewModel;
}

void NordnetModel::addTrigger(Trigger* pTrigger)
{
    m_pTriggers.append(pTrigger);
}

void NordnetModel::remove()
{
  QByteArray loginData = getLoginData();
  QByteArray json2 = getUnSubscriptionString();
  qWarning() << "json string: " << QString(loginData.data());
  qWarning() << "json2 string: " << QString(json2.data());
  if(m_pSession){
//      NordnetStream* pNordnetStream = new NordnetStream();
//      pNordnetStream->socketConnect(loginData, m_pSession->getPublicHost(), m_pSession->getPublicPort());
      m_pNordnetStream->startSubscription(json2);
  }else{
      qWarning() << "Error: No session - failed to get stock data!";
  }
}

QString NordnetModel::getTicker(QString market, QString identifier) {
    if(mStocksMarketMap.contains(market)){
        QList<Stock*> stocks = mStocksMarketMap.value(market);
        for(int i = 0; i < stocks.size(); i++){
            if(stocks.at(i)->getIdentifier() == identifier){
                return stocks.at(i)->getName();
            }
        }
    }
    return "ERROR";
}
