QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnuitar-qt

TEMPLATE = app

INCLUDEPATH += ..

SOURCES += "gnuitar-qt.cc"

SOURCES += "audio-panel.cc"
SOURCES += "button.cc"
SOURCES += "controller.cc"
SOURCES += "main-window.cc"
SOURCES += "menu-bar.cc"
SOURCES += "doc-browser.cc"
SOURCES += "knob.cc"
SOURCES += "rack.cc"
SOURCES += "effect-view.cc"

HEADERS += "audio-panel.h"
HEADERS += "button.h"
HEADERS += "controller.h"
HEADERS += "main-window.h"
HEADERS += "menu-bar.h"
HEADERS += "doc-browser.h"
HEADERS += "knob.h"
HEADERS += "rack.h"
HEADERS += "effect-view.h"

RESOURCES += "resources.qrc"

unix {
  LIBS += ../libgnuitar/libgnuitar.a -ldl -lpthread -lasound
}

win32 {
  LIBS += ../libgnuitar/gnuitar.lib
}

