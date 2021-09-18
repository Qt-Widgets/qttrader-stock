TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += Volume.h
SOURCES += Volume.cpp
HEADERS += VolumeDialog.h
SOURCES += VolumeDialog.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/Volume
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network



!exists(./build){
  system(echo mkdir build)
}
