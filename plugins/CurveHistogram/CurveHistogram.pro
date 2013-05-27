TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += CurveHistogram.h
SOURCES += CurveHistogram.cpp
HEADERS += CHBar.h
SOURCES += CHBar.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/CurveHistogram
INSTALLS += target

QT += core
QT += gui

!exists(./build){
  system(echo mkdir build)
}
