#ifndef NORDNETMODEL_H
#define NORDNETMODEL_H

#include "Account.h"
#include <QObject>
#include "orderviewmodel.h"
#include "positionviewmodel.h"
#include "tradeviewmodel.h"
#include "trigger.h"
#include <QMap>

class NordnetAuthenticated;
class Session;
class Ledger;
class Order;
class Position;
class Trade;
class Market;
class Stock;
class Index;
class PriceTick;
class NordnetStream;
class QString;
class QVariant;
class QNetworkReply;
class QByteArray;


class NordnetModel : public QObject {

    Q_OBJECT

public:
    NordnetModel();
    bool isAutenticated();

    QList<Ledger*> mLedgers;
    QList<Trade*> mTrades;
    QList<Order*> mOrders;
    QList<Position*> mPositions;
    QList<Account*> mAccounts;

    QList<Market*> mMarkets;
    QList<Stock*> mStocks;
    QList<Index*> mIndex;

    QList<PriceTick*> mPriceTicks;

    void addTrigger(Trigger* pTrigger);

public slots:
    void getAccounts();
    void getMarkets();
    void getStocks(int pos);
    void getStockData(int pos);
    void getChartForStock(int stockPosition);
    void test();
    void login (QString userName, QString password);
    void logout();
    void placeOrder();
    void placeSellOrder(int identifier, QString volume, QString price);
    void placeBuyOrder(int identifier, QString volume, QString price);

    void onTest(QNetworkReply *reply);

    void onAccount(QNetworkReply *reply);
    void onAccountSummary(QNetworkReply *reply);
    void onTrade(QNetworkReply *reply);
    void onOrder(QNetworkReply *reply);
    void onLedger(QNetworkReply *reply);
    void onPosition(QNetworkReply *reply);

    void onChart(QNetworkReply *reply);
    void onStocks(QNetworkReply *reply, int marketID);
    void onMarkets(QNetworkReply *reply);
    void onIndex(QNetworkReply *reply);

    void onLogin(QNetworkReply *reply);
    void onLogout(QNetworkReply * reply);

    void onPriceTick(QByteArray reply);

    void remove();

    OrderViewModel* getOrderViewModel();
    TradeViewModel* getTradeViewModel();
    PositionViewModel* getPositionViewModel();


signals:
    void answer(QString);
    void loginDone();
    void accountsDone();
    void marketsDone();
    void stocksDone();
    void tradeTick();
    void orderModelUpdated();
    void positionModelUpdated();

private:
    void tickToBars();
    QVariant parseStream(QByteArray array);
    QVariant parseReply(QNetworkReply *reply);
    QByteArray getSubscriptionString(int pos);
    QByteArray getUnSubscriptionString();
    QByteArray getLoginData();
    Session* m_pSession;
    OrderViewModel* m_pOrderViewModel;
    TradeViewModel* m_pTradeViewModel;
    PositionViewModel* m_pPositionViewModel;
    QList<Trigger*> m_pTriggers;
    QMap<QString, QList<Stock*> > mStocksMap;
    int mPosition;
    NordnetStream* m_pNordnetStream;
};

#endif // NORDNETMODEL_H
