TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += CurveLine.h
SOURCES += CurveLine.cpp
HEADERS += CurveLineBar.h
SOURCES += CurveLineBar.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/CurveLine
INSTALLS += target

QT += core
QT += gui

!exists(./build){
  system(echo mkdir build)
}
