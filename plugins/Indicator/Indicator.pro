
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lQTTRADER -lta_lib
MOC_DIR += build
OBJECTS_DIR += build

HEADERS += ControlWidget.h \
    ToolbarWidget.h
SOURCES += ControlWidget.cpp \
    ToolbarWidget.cpp
HEADERS += Indicator.h
SOURCES += Indicator.cpp
HEADERS += IndicatorDialog.h
SOURCES += IndicatorDialog.cpp
HEADERS += LengthButton.h
SOURCES += LengthButton.cpp
HEADERS += PlotWidget.h
SOURCES += PlotWidget.cpp

target.path = "$${INSTALL_PLUGIN_DIR}"/Indicator
INSTALLS += target

QT += core
QT += gui
QT += sql
QT += network

OTHER_FILES += \
    gui.qml

!exists(./build){
  system(echo mkdir build)
}

RESOURCES += \
    ../../icons.qrc
