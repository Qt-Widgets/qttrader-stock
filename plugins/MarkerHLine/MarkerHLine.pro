TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += MarkerHLine.h
SOURCES += MarkerHLine.cpp
HEADERS += MarkerHLineDialog.h
SOURCES += MarkerHLineDialog.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/MarkerHLine
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}
