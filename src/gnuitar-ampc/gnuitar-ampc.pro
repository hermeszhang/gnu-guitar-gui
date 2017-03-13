TEMPLATE = app

QT -= gui core

INCPATH += ".."

TARGET = gnuitar-ampc

HEADERS += "capacitor.h"
HEADERS += "circuit.h"
HEADERS += "component.h"
HEADERS += "resistor.h"
HEADERS += "source-writer.h"
HEADERS += "visitor.h"
HEADERS += "lexer.h"
HEADERS += "spice-token.h"
HEADERS += "spice-lexer.h"
HEADERS += "spice-parser.h"
HEADERS += "exception.h"

SOURCES += "capacitor.cc"
SOURCES += "circuit.cc"
SOURCES += "component.cc"
SOURCES += "resistor.cc"
SOURCES += "source-writer.cc"
SOURCES += "visitor.cc"
SOURCES += "lexer.cc"
SOURCES += "spice-token.cc"
SOURCES += "spice-lexer.cc"
SOURCES += "spice-parser.cc"
SOURCES += "exception.cc"

SOURCES += "gnuitar-ampc.cc"

