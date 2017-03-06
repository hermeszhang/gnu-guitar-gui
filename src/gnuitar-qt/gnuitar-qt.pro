QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnuitar-qt

TEMPLATE = app

INCLUDEPATH += ..

SOURCES += "gnuitar-qt.cc"

SOURCES += "audio-panel.cc"
SOURCES += "button.cc"
SOURCES += "main-window.cc"
SOURCES += "menu-bar.cc"
SOURCES += "doc-browser.cc"
SOURCES += "knob.cc"
SOURCES += "rack.cc"

HEADERS += "audio-panel.h"
HEADERS += "button.h"
HEADERS += "main-window.h"
HEADERS += "menu-bar.h"
HEADERS += "doc-browser.h"
HEADERS += "knob.h"
HEADERS += "rack.h"

RESOURCES += "resources.qrc"

unix {
  LIBS += ../libgnuitar/libgnuitar.a -ldl -lpthread -lasound
}

win32 {
  LIBS += ../libgnuitar/gnuitar.lib
}

