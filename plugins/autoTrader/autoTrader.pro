TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += ../../lib/QtTraderPlugin.h
HEADERS += autoTrader.h
SOURCES += autoTrader.cpp
HEADERS += autoTraderDialog.h
SOURCES += autoTraderDialog.cpp

target.path =  "$${INSTALL_PLUGIN_DIR}"
INSTALLS += target

QT += core
QT += gui

!exists(./build){
  system(echo mkdir build)
}
