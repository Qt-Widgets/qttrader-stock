/*
 *  Qtstalker stock charter
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */

#ifndef PLUGIN_NORDNET_GUI_WIDGET_H
#define PLUGIN_NORDNET_GUI_WIDGET_H

#include <QStringList>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <QString>
#include <QTabWidget>
#include <QComboBox>
#include "Widget.h"

class NordnetModel;

class GUIWidget : public Widget
{
  Q_OBJECT

  public:
    GUIWidget (NordnetModel*);
    ~GUIWidget ();
    void createGUI ();


  public slots:
    void login ();
    void updateGUI ();
    void answer(QString);
    void showLoginDialog();
    void onLogin();
    void onAccounts();
    void onMarkets();
    void onStocks();
    void onTrade();
    void placeOrder();
    void onOrderModelUpdated();
    void onTradeModelUpdated();
    void onPositionModelUpdated();
    void addTrigger();
    void stocksRequested();

  signals:
    void placeSellOrder(int,QString,QString);
    void placeBuyOrder(int,QString,QString);

  private:

    QPushButton *_okButton;

    QLineEdit* m_username;
    QLineEdit* m_password;

    QTextEdit *_log;
    QTimer *_timer;

    NordnetModel* m_pModel;
    QComboBox* m_pRequester;
};

#endif //PLUGIN_NORDNET_GUI_WIDGET_H
