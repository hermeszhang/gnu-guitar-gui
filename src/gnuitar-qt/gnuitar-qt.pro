QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnuitar-qt

TEMPLATE = app

SOURCES += "main.cpp"
SOURCES += "main-window.cpp"
SOURCES += "doc-browser.cpp"

HEADERS += "main-window.h"
HEADERS += "doc-browser.h"

FORMS += "main-window.ui"

CFLAGS += -Wall -Wextra -Werror -Wfatal-errors -std=c++11

LIBS +=  ../libgnuitar.so
LIBS += -lasound

