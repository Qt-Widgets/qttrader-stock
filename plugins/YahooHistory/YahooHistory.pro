TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += GUIWidget.h \
    yahoodownload.h \
    dialog.h
SOURCES += GUIWidget.cpp \
    yahoodownload.cpp \
    dialog.cpp
HEADERS += YahooHistory.h
SOURCES += YahooHistory.cpp
HEADERS += YahooHistoryDownload.h
SOURCES += YahooHistoryDownload.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/YahooHistory
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}
