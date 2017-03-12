QT -= gui core

TEMPLATE = lib
CONFIG = staticlib
VERSION = 0.4
TARGET = gnuitar

INCLUDEPATH += "$$PWD/.."

SOURCES += "driver.cc"
SOURCES += "driver-manager.cc"
SOURCES += "effect.cc"
SOURCES += "effect-list.cc"
SOURCES += "exception.cc"
SOURCES += "shared-library.cc"
SOURCES += "shared-library-collection.cc"

HEADERS += "driver.h"
HEADERS += "driver-manager.h"
HEADERS += "effect.h"
HEADERS += "effect-list.h"
SOURCES += "exception.h"
HEADERS += "shared-library.h"
HEADERS += "shared-library-collection.cc"

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

