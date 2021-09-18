TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += MarkerSell.h
SOURCES += MarkerSell.cpp
HEADERS += MarkerSellDialog.h
SOURCES += MarkerSellDialog.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/MarkerSell
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}
