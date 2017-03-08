TEMPLATE = app

QT -= core gui

INCLUDEPATH += ".."

SOURCES += "gnuitar-sh.cc"
SOURCES += "shell.cc"

HEADERS += "shell.h"

unix {
  LIBS += ../libgnuitar/libgnuitar.a
  LIBS += -ldl -lpthread -lasound
}

win32 {
  LIBS += ../libgnuitar/gnuitar.lib
}

