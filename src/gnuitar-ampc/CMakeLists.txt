CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT ("gnuitar-ampc")

INCLUDE_DIRECTORIES ("..")

ADD_LIBRARY ("ampc"
  "capacitor.h"
  "capacitor.cc"
  "circuit.h"
  "circuit.cc"
  "exception.h"
  "exception.cc"
  "component.h"
  "component.cc"
  "resistor.h"
  "resistor.cc"
  "source-writer.h"
  "source-writer.cc"
  "lexer.h"
  "lexer.cc"
  "spice-token.h"
  "spice-token.cc"
  "spice-lexer.h"
  "spice-lexer.cc"
  "spice-parser.h"
  "spice-parser.cc"
  "visitor.h"
  "visitor.cc")

ADD_EXECUTABLE ("gnuitar-ampc" "gnuitar-ampc.cc")
TARGET_LINK_LIBRARIES ("gnuitar-ampc" "ampc")

FUNCTION (ADD_AMPC_TEST TEST_NAME)
  ADD_EXECUTABLE (${TEST_NAME} ${ARGN})
  TARGET_LINK_LIBRARIES (${TEST_NAME} "ampc")
  ADD_TEST(${TEST_NAME} ${TEST_NAME})
ENDFUNCTION (ADD_AMPC_TEST TEST_NAME)

ENABLE_TESTING ()

ADD_AMPC_TEST ("spice-lexer-test" "spice-lexer-test.cc")
ADD_AMPC_TEST ("spice-parser-test" "spice-parser-test.cc")

