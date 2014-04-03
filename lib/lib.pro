######################################################
# SOURCE
#####################################################

HEADERS += bar/Bar.h
SOURCES += bar/Bar.cpp
HEADERS += bar/Bars.h
SOURCES += bar/Bars.cpp
HEADERS += bar/BarLength.h
SOURCES += bar/BarLength.cpp
HEADERS += bar/BarType.h
SOURCES += bar/BarType.cpp
HEADERS += bar/CBar.h
SOURCES += bar/CBar.cpp

HEADERS += QtTraderPlugin.h

HEADERS += ColorButton.h
SOURCES += ColorButton.cpp
HEADERS += curve/Curve.h
SOURCES += curve/Curve.cpp
HEADERS += curve/CurveHistogramType.h
SOURCES += curve/CurveHistogramType.cpp
HEADERS += curve/CurveLineType.h
SOURCES += curve/CurveLineType.cpp
HEADERS += curve/CurveOHLCType.h
SOURCES += curve/CurveOHLCType.cpp
HEADERS += DataBase.h
SOURCES += DataBase.cpp
HEADERS += DateRange.h
SOURCES += DateRange.cpp
HEADERS += Dialog.h
SOURCES += Dialog.cpp
HEADERS += Entity.h
SOURCES += Entity.cpp
HEADERS += Global.h
SOURCES += Global.cpp
HEADERS += MAType.h
SOURCES += MAType.cpp
HEADERS += Marker.h
SOURCES += Marker.cpp
HEADERS += plot/Plot.h
SOURCES += plot/Plot.cpp
HEADERS += plot/PlotDateScaleDraw.h
SOURCES += plot/PlotDateScaleDraw.cpp
HEADERS += plot/PlotInfo.h
SOURCES += plot/PlotInfo.cpp
HEADERS += plot/PlotPicker.h
SOURCES += plot/PlotPicker.cpp
HEADERS += plot/PlotScaleDraw.h
SOURCES += plot/PlotScaleDraw.cpp
HEADERS += plot/PlotSettings.h
HEADERS += plot/PlotStatus.h
HEADERS += PluginFactory.h
SOURCES += PluginFactory.cpp
HEADERS += Widget.h
SOURCES += Widget.cpp

######################################################
# QMAKE SETTINGS
#####################################################

TEMPLATE = lib
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
CONFIG += thread warn_on
CONFIG(debug, debug|release) {
    message("Building lib in debug mode")
    DEFINES += DEBUG _DEBUG
}

QMAKE_CXXFLAGS += -rdynamic -ffast-math

TARGET = QTTRADER

VERSION = 0.1.0


INCLUDEPATH += /usr/include/qwt
INCLUDEPATH += /usr/local/include/ta-lib
INCLUDEPATH += ./bar
INCLUDEPATH += ./plot
INCLUDEPATH += ./curve

LIBS += -L/usr/lib

message("Using INCLUDEPATH=$$INCLUDEPATH")
message("Using LIBS=$$LIBS")
target.path = "$${INSTALL_LIB_DIR}"
message("Using target.path = $$target.path")
message("Using INSTALL_LIB_DIR = $$INSTALL_LIB_DIR")

INSTALLS += target

QMAKE_DISTCLEAN += qttrader_defines.h
QMAKE_DISTCLEAN += ../.qmake.cache


MOC_DIR += build
OBJECTS_DIR += build

QT += core
QT += gui
QT += sql
QT += network

RESOURCES += \
    ../icons.qrc

!exists(./build){
  system(echo $(mkdir) build)
}
