QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnuitar-qt

TEMPLATE = app

SOURCES += "gnuitar-qt.cc"
SOURCES += "main-window.cc"
SOURCES += "doc-browser.cc"
SOURCES += "knob.cc"
SOURCES += "rack.cc"

HEADERS += "main-window.h"
HEADERS += "doc-browser.h"
HEADERS += "knob.h"
HEADERS += "rack.h"

FORMS += "main-window.ui"

CFLAGS += -Wall -Wextra -Werror -Wfatal-errors -std=c++11

LIBS +=  ../libgnuitar.so
LIBS += -lasound

