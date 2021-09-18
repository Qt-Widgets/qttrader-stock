#
# DO NOT USE caching for now. Does not help in build times, but creates stupid problems during development.
#
#exists(.qmake.cache) {
#  message("Using .qmake.cache")
#}
#else {
  message("Doing automated configuration ...")

  unix {
    MY_OS = "unix"
    INCLUDEPATH += /usr/include/qt4/Qt
    exists(/usr/local/include/ta-lib) {
      INCLUDEPATH += /usr/local/include/ta-lib
    }
    exists(/usr/include/ta-lib) {
      INCLUDEPATH += /usr/include/ta-lib
    }
    INCLUDEPATH += /usr/include/qwt
  }

  freebsd-g++ {
    MY_OS = "freebsd"
    INCLUDEPATH += /usr/X11R6/include
  }

  linux-g++ {
    MY_OS = "linux"
  }

  win32 {
    MY_OS = "windows"
  }

  macx {
    MY_OS = "macx"
    # For MacPorts
    exists(/opt/local/include/ta-lib) {
      INCLUDEPATH += /opt/local/include/ta-lib 
    }
    exists(/opt/local/lib) {
      LIBS += -L/opt/local/lib
    }
    # For Fink
    exists(/sw/include/ta-lib) {
      INCLUDEPATH += /sw/include/ta-lib
    }
    exists(/sw/lib) {
      LIBS += -L/sw/lib
    }
  }


  !exists(qttrader.config) {
	error("Missing configuration file qttrader.config")
  }
	include(qttrader.config)
  
  INCLUDEPATH = $$unique(INCLUDEPATH)
  LIBS = $$unique(LIBS)

  message("Writing .qmake.cache")
  system(echo "INCLUDEPATH=$$INCLUDEPATH" > .qmake.cache)
  system(echo "LIBS=$$LIBS" >> .qmake.cache)
  system(echo "MY_OS=$$MY_OS" >> .qmake.cache)
  system(echo "INSTALL_BIN_DIR=$$INSTALL_BIN_DIR" >> .qmake.cache)
  system(echo "INSTALL_DATA_DIR=$$INSTALL_DATA_DIR" >> .qmake.cache)
  system(echo "INSTALL_DOCS_DIR=$$INSTALL_DOCS_DIR" >> .qmake.cache)
  system(echo "INSTALL_LIB_DIR=$$INSTALL_LIB_DIR" >> .qmake.cache)
  system(echo "INSTALL_PLUGIN_DIR=$$INSTALL_PLUGIN_DIR" >> .qmake.cache)

  message("Writing lib/qttrader_defines.h")
  win32 {
    system(echo //WARNING! THIS FILE IS AUTO GENERATED. DO NOT EDIT > lib/qttrader_defines.h)
    system(echo //Changes are to be made in qttrader.pro. Then re-run ./configure to generate this file >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}ifndef qttrader_DEFINES_H >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}define qttrader_DEFINES_H >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}define INSTALL_BIN_DIR \"$$INSTALL_BIN_DIR\" >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}define INSTALL_DATA_DIR \"$$INSTALL_DATA_DIR\" >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}define INSTALL_DOCS_DIR \"$$INSTALL_DOCS_DIR\" >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}define INSTALL_LIB_DIR \"$$INSTALL_LIB_DIR\" >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}define INSTALL_PLUGIN_DIR \"$$INSTALL_PLUGIN_DIR\" >> lib/qttrader_defines.h)
    system(echo $${LITERAL_HASH}endif >> lib/qttrader_defines.h)
  } else {
    system(echo \"//WARNING! THIS FILE IS AUTO GENERATED. DO NOT EDIT\" > lib/qttrader_defines.h)
    system(echo \"//Changes are to be made in qttrader.pro. Then re-run ./configure to generate this file\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}ifndef QTTRADER_DEFINES_H\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}define QTTRADER_DEFINES_H\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}define INSTALL_BIN_DIR \\\"$$INSTALL_BIN_DIR\\\"\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}define INSTALL_DATA_DIR \\\"$$INSTALL_DATA_DIR\\\"\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}define INSTALL_DOCS_DIR \\\"$$INSTALL_DOCS_DIR\\\"\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}define INSTALL_LIB_DIR \\\"$$INSTALL_LIB_DIR\\\"\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}define INSTALL_PLUGIN_DIR \\\"$$INSTALL_PLUGIN_DIR\\\"\" >> lib/qttrader_defines.h)
    system(echo \"$${LITERAL_HASH}endif\" >> lib/qttrader_defines.h)
  }
#}

message("Using INCLUDEPATH=$$INCLUDEPATH")
message("Using LIBS=$$LIBS")
message("Operating system: $$MY_OS")
!isEmpty(QMAKESPEC) {
  message("QMAKESPEC: $$QMAKESPEC")
}

TEMPLATE = subdirs

# compile qttrader lib
SUBDIRS += lib

# compile app
SUBDIRS += src

# compile plugins
#SUBDIRS += plugins/CDL
SUBDIRS += plugins/CurveLine
SUBDIRS += plugins/CurveHistogram
SUBDIRS += plugins/CurveOHLC
SUBDIRS += plugins/Database
SUBDIRS += plugins/Indicator
SUBDIRS += plugins/MarkerBuy
SUBDIRS += plugins/MarkerSell
SUBDIRS += plugins/MarkerTLine
SUBDIRS += plugins/MarkerHLine
SUBDIRS += plugins/Volume
SUBDIRS += plugins/YahooHistory

SUBDIRS += plugins/autoTrader
SUBDIRS += plugins/NetfondsHistory
SUBDIRS += plugins/Nordnet

!win32 {
  # install docs and i18n
 # SUBDIRS += docs
}
