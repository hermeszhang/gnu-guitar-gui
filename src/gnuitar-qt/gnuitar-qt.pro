QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnuitar-qt

TEMPLATE = app

SOURCES += "main.cpp" "main-window.cpp"

HEADERS += "main-window.h"

FORMS += "main-window.ui"

CFLAGS += -Wall -Wextra -Werror -Wfatal-errors -std=c++11

LIBS +=  ../libgnuitar.so
LIBS += -lasound

