TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += CurveOHLC.h
SOURCES += CurveOHLC.cpp
HEADERS += OHLCBar.h
SOURCES += OHLCBar.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/CurveOHLC
INSTALLS += target

QT += core
QT += gui

!exists(./build){
  system(echo mkdir build)
}
