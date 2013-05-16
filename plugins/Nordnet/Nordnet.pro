TEMPLATE = lib
CONFIG += plugin
CONFIG += uitools
INCLUDEPATH += ../../lib
INCLUDEPATH += ./messages
LIBS += -L../../lib -lQTTRADER -lssl -lcrypto -lqjson

MOC_DIR += build
OBJECTS_DIR += build

HEADERS += Nordnet.h \
    GUIWidget.h \
    NordnetModel.h \
    NordnetAuthenticated.h \
    nordnetstream.h \
    Session.h \
    messages/Account.h \
    messages/position.h \
    messages/ledger.h \
    messages/order.h \
    messages/trade.h \
    messages/market.h \
    messages/stock.h \
    messages/index.h \
    messages/tick.h \
    messages/indextick.h \
    messages/pricetick.h \
    messages/depthtick.h \
    orderviewmodel.h \
    tradeviewmodel.h \
    positionviewmodel.h \
    trigger.h
SOURCES += GUIWidget.cpp \
    Nordnet.cpp \
    Session.cpp \
    NordnetModel.cpp \
    NordnetAuthenticated.cpp \
    nordnetstream.cpp \
    messages/Account.cpp \
    messages/position.cpp \
    messages/ledger.cpp \
    messages/order.cpp \
    messages/trade.cpp \
    messages/market.cpp \
    messages/stock.cpp \
    messages/index.cpp \
    messages/tick.cpp \
    messages/indextick.cpp \
    messages/pricetick.cpp \
    messages/depthtick.cpp \
    orderviewmodel.cpp \
    tradeviewmodel.cpp \
    positionviewmodel.cpp \
    trigger.cpp

target.path =  "$${INSTALL_PLUGIN_DIR}"/Nordnet
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}

FORMS += \
    tabwidget.ui \
    logindialog.ui

RESOURCES += \
    forms.qrc
