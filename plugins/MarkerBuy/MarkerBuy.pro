TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += MarkerBuy.h
SOURCES += MarkerBuy.cpp
HEADERS += MarkerBuyDialog.h
SOURCES += MarkerBuyDialog.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/MarkerBuy
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}
