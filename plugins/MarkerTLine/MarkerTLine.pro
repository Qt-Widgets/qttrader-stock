TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += MarkerTLine.h
SOURCES += MarkerTLine.cpp
HEADERS += MarkerTLineDialog.h
SOURCES += MarkerTLineDialog.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/MarkerTLine
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

!exists(./build){
  system(echo mkdir build)
}
