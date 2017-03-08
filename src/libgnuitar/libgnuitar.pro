QT -= gui core

TEMPLATE = lib
CONFIG = staticlib
VERSION = 0.4
TARGET = gnuitar

INCLUDEPATH += "$$PWD/.."

SOURCES += "driver.cc"
SOURCES += "plugin.cc"

HEADERS += "driver.h"
HEADERS += "plugin.h"

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
