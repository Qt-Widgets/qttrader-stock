TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER

MOC_DIR += build
OBJECTS_DIR += build

HEADERS += GUIWidget.h
SOURCES += GUIWidget.cpp
HEADERS += NetfondsHistory.h
SOURCES += NetfondsHistory.cpp
HEADERS += NetfondsHistoryDownload.h
SOURCES += NetfondsHistoryDownload.cpp

target.path =  "$${INSTALL_PLUGIN_DIR}"
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}
