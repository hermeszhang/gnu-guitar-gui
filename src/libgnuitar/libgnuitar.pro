QT -= gui core

TEMPLATE = lib
CONFIG = staticlib
VERSION = 0.4
TARGET = gnuitar

INCLUDEPATH += "$$PWD/.."

SOURCES += "driver.cc"
SOURCES += "plugin.cc"
SOURCES += "plugin-manager.cc"

HEADERS += "../include/gnuitar/driver.h"
HEADERS += "../include/gnuitar/plugin.h"
HEADERS += "../include/gnuitar/plugin-manager.h"

packagesExist(alsa) {
  SOURCES += "alsa-driver.cc"
  HEADERS += "alsa-driver.h"
  LIBS += -lasound
  DEFINES += GNUITAR_WITH_ALSA=1
}

g++ {
  CXXFLAGS += -Wall -Wextra -Werror -Wfatal-errors
  CXXFLAGS += -std=c++11
}

linux-g++ {
  LIBS += -ldl -lpthread
}

SUBDIRS += "gnuitar-qt"

