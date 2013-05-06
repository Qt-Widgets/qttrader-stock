/********************************************************************************
** Form generated from reading UI file 'tabwidget.ui'
**
** Created: Tue Apr 30 19:03:12 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABWIDGET_H
#define UI_TABWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabWidget
{
public:
    QWidget *konto;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QLabel *balans;
    QLabel *label_4;
    QLabel *sum_stocks;
    QTableView *orderView;
    QPushButton *logout;
    QTableView *positionView;
    QTableView *tradeView;
    QWidget *aktier;
    QLabel *label_3;
    QComboBox *marketsList;
    QLabel *label_5;
    QComboBox *stocksList;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *bid;
    QLabel *ask;
    QLabel *latest;
    QLabel *volume;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *sum;
    QLineEdit *price;
    QPushButton *sellButton;
    QPushButton *buyButton;
    QGroupBox *groupBox;
    QLabel *label_10;
    QComboBox *triggerMarketsList;
    QComboBox *triggerStocksList;
    QLabel *label_11;
    QComboBox *triggerSide;
    QLabel *label_12;
    QLineEdit *triggerLevel;
    QLabel *label_13;
    QFrame *line;
    QLabel *label_16;
    QLineEdit *orderVolume;
    QLineEdit *orderPrice;
    QLabel *label_19;
    QComboBox *orderSide;
    QPushButton *triggerButton;
    QComboBox *orderStocksList;
    QComboBox *orderMarketsList;
    QLabel *label_17;
    QLabel *label_18;
    QWidget *order;
    QPushButton *startSub;

    void setupUi(QTabWidget *TabWidget)
    {
        if (TabWidget->objectName().isEmpty())
            TabWidget->setObjectName(QString::fromUtf8("TabWidget"));
        TabWidget->resize(983, 733);
        konto = new QWidget();
        konto->setObjectName(QString::fromUtf8("konto"));
        formLayout = new QFormLayout(konto);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(konto);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        comboBox = new QComboBox(konto);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBox);

        label_2 = new QLabel(konto);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        balans = new QLabel(konto);
        balans->setObjectName(QString::fromUtf8("balans"));

        formLayout->setWidget(2, QFormLayout::FieldRole, balans);

        label_4 = new QLabel(konto);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        sum_stocks = new QLabel(konto);
        sum_stocks->setObjectName(QString::fromUtf8("sum_stocks"));

        formLayout->setWidget(3, QFormLayout::FieldRole, sum_stocks);

        orderView = new QTableView(konto);
        orderView->setObjectName(QString::fromUtf8("orderView"));
        orderView->setEnabled(true);
        orderView->setGridStyle(Qt::DashLine);
        orderView->setSortingEnabled(true);
        orderView->setWordWrap(true);
        orderView->horizontalHeader()->setCascadingSectionResizes(true);
        orderView->horizontalHeader()->setDefaultSectionSize(150);
        orderView->horizontalHeader()->setStretchLastSection(true);
        orderView->verticalHeader()->setDefaultSectionSize(25);

        formLayout->setWidget(5, QFormLayout::SpanningRole, orderView);

        logout = new QPushButton(konto);
        logout->setObjectName(QString::fromUtf8("logout"));

        formLayout->setWidget(8, QFormLayout::LabelRole, logout);

        positionView = new QTableView(konto);
        positionView->setObjectName(QString::fromUtf8("positionView"));

        formLayout->setWidget(6, QFormLayout::SpanningRole, positionView);

        tradeView = new QTableView(konto);
        tradeView->setObjectName(QString::fromUtf8("tradeView"));

        formLayout->setWidget(7, QFormLayout::SpanningRole, tradeView);

        TabWidget->addTab(konto, QString());
        aktier = new QWidget();
        aktier->setObjectName(QString::fromUtf8("aktier"));
        label_3 = new QLabel(aktier);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 20, 66, 17));
        marketsList = new QComboBox(aktier);
        marketsList->setObjectName(QString::fromUtf8("marketsList"));
        marketsList->setGeometry(QRect(130, 10, 251, 27));
        label_5 = new QLabel(aktier);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 60, 91, 17));
        stocksList = new QComboBox(aktier);
        stocksList->setObjectName(QString::fromUtf8("stocksList"));
        stocksList->setGeometry(QRect(130, 50, 251, 27));
        label_6 = new QLabel(aktier);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 110, 66, 17));
        label_7 = new QLabel(aktier);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(160, 110, 66, 17));
        label_8 = new QLabel(aktier);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(270, 110, 66, 17));
        label_9 = new QLabel(aktier);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(380, 110, 66, 17));
        bid = new QLabel(aktier);
        bid->setObjectName(QString::fromUtf8("bid"));
        bid->setGeometry(QRect(20, 130, 66, 17));
        ask = new QLabel(aktier);
        ask->setObjectName(QString::fromUtf8("ask"));
        ask->setGeometry(QRect(160, 130, 66, 17));
        latest = new QLabel(aktier);
        latest->setObjectName(QString::fromUtf8("latest"));
        latest->setGeometry(QRect(270, 130, 66, 17));
        volume = new QLabel(aktier);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setGeometry(QRect(380, 130, 66, 17));
        label_14 = new QLabel(aktier);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 200, 66, 17));
        label_15 = new QLabel(aktier);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(20, 240, 66, 17));
        sum = new QLineEdit(aktier);
        sum->setObjectName(QString::fromUtf8("sum"));
        sum->setGeometry(QRect(80, 190, 113, 27));
        price = new QLineEdit(aktier);
        price->setObjectName(QString::fromUtf8("price"));
        price->setGeometry(QRect(80, 230, 113, 27));
        sellButton = new QPushButton(aktier);
        sellButton->setObjectName(QString::fromUtf8("sellButton"));
        sellButton->setGeometry(QRect(210, 190, 98, 27));
        buyButton = new QPushButton(aktier);
        buyButton->setObjectName(QString::fromUtf8("buyButton"));
        buyButton->setGeometry(QRect(210, 230, 98, 27));
        groupBox = new QGroupBox(aktier);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 280, 431, 401));
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 80, 151, 17));
        triggerMarketsList = new QComboBox(groupBox);
        triggerMarketsList->setObjectName(QString::fromUtf8("triggerMarketsList"));
        triggerMarketsList->setGeometry(QRect(160, 30, 251, 27));
        triggerStocksList = new QComboBox(groupBox);
        triggerStocksList->setObjectName(QString::fromUtf8("triggerStocksList"));
        triggerStocksList->setGeometry(QRect(160, 70, 251, 27));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(40, 40, 111, 17));
        triggerSide = new QComboBox(groupBox);
        triggerSide->setObjectName(QString::fromUtf8("triggerSide"));
        triggerSide->setGeometry(QRect(160, 110, 251, 27));
        triggerSide->setMaxVisibleItems(6);
        triggerSide->setMaxCount(6);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(70, 110, 81, 31));
        triggerLevel = new QLineEdit(groupBox);
        triggerLevel->setObjectName(QString::fromUtf8("triggerLevel"));
        triggerLevel->setGeometry(QRect(160, 150, 113, 27));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(60, 160, 91, 20));
        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 190, 391, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(90, 210, 66, 17));
        orderVolume = new QLineEdit(groupBox);
        orderVolume->setObjectName(QString::fromUtf8("orderVolume"));
        orderVolume->setGeometry(QRect(160, 200, 113, 27));
        orderPrice = new QLineEdit(groupBox);
        orderPrice->setObjectName(QString::fromUtf8("orderPrice"));
        orderPrice->setGeometry(QRect(160, 230, 113, 27));
        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(105, 240, 51, 20));
        orderSide = new QComboBox(groupBox);
        orderSide->setObjectName(QString::fromUtf8("orderSide"));
        orderSide->setGeometry(QRect(280, 200, 131, 27));
        triggerButton = new QPushButton(aktier);
        triggerButton->setObjectName(QString::fromUtf8("triggerButton"));
        triggerButton->setGeometry(QRect(180, 620, 98, 27));
        orderStocksList = new QComboBox(aktier);
        orderStocksList->setObjectName(QString::fromUtf8("orderStocksList"));
        orderStocksList->setGeometry(QRect(180, 580, 251, 27));
        orderMarketsList = new QComboBox(aktier);
        orderMarketsList->setObjectName(QString::fromUtf8("orderMarketsList"));
        orderMarketsList->setGeometry(QRect(180, 540, 251, 27));
        label_17 = new QLabel(aktier);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(80, 590, 91, 17));
        label_18 = new QLabel(aktier);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(110, 550, 66, 17));
        TabWidget->addTab(aktier, QString());
        order = new QWidget();
        order->setObjectName(QString::fromUtf8("order"));
        startSub = new QPushButton(order);
        startSub->setObjectName(QString::fromUtf8("startSub"));
        startSub->setGeometry(QRect(40, 40, 98, 27));
        TabWidget->addTab(order, QString());

        retranslateUi(TabWidget);

        TabWidget->setCurrentIndex(1);
        triggerSide->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(TabWidget);
    } // setupUi

    void retranslateUi(QTabWidget *TabWidget)
    {
        TabWidget->setWindowTitle(QApplication::translate("TabWidget", "TabWidget", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TabWidget", "Konto:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TabWidget", "Balans:", 0, QApplication::UnicodeUTF8));
        balans->setText(QApplication::translate("TabWidget", "0 SEK", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TabWidget", "V\303\244rdepapper:", 0, QApplication::UnicodeUTF8));
        sum_stocks->setText(QApplication::translate("TabWidget", "0 SEK", 0, QApplication::UnicodeUTF8));
        logout->setText(QApplication::translate("TabWidget", "Logga ut", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(konto), QApplication::translate("TabWidget", "Konto", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TabWidget", "Marknad:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TabWidget", "V\303\244rdepapper:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TabWidget", "K\303\266p", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TabWidget", "S\303\244lj", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("TabWidget", "Senast", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("TabWidget", "Volym", 0, QApplication::UnicodeUTF8));
        bid->setText(QApplication::translate("TabWidget", "0", 0, QApplication::UnicodeUTF8));
        ask->setText(QApplication::translate("TabWidget", "0", 0, QApplication::UnicodeUTF8));
        latest->setText(QApplication::translate("TabWidget", "0", 0, QApplication::UnicodeUTF8));
        volume->setText(QApplication::translate("TabWidget", "0", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("TabWidget", "Antal:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("TabWidget", "Pris:", 0, QApplication::UnicodeUTF8));
        sellButton->setText(QApplication::translate("TabWidget", "S\303\244lj", 0, QApplication::UnicodeUTF8));
        buyButton->setText(QApplication::translate("TabWidget", "K\303\266p", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TabWidget", "Trigger:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("TabWidget", "Trigger V\303\244rdepapper:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("TabWidget", "Trigger Marknad:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("TabWidget", "Tigger side:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("TabWidget", "Trigger Level:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("TabWidget", "Volume:", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("TabWidget", "Price:", 0, QApplication::UnicodeUTF8));
        triggerButton->setText(QApplication::translate("TabWidget", "Add Trigger", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("TabWidget", "V\303\244rdepapper:", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("TabWidget", "Marknad:", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(aktier), QApplication::translate("TabWidget", "Aktier", 0, QApplication::UnicodeUTF8));
        startSub->setText(QApplication::translate("TabWidget", "PushButton", 0, QApplication::UnicodeUTF8));
        TabWidget->setTabText(TabWidget->indexOf(order), QApplication::translate("TabWidget", "Order", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TabWidget: public Ui_TabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABWIDGET_H
