CMAKE_MINIMUM_REQUIRED (VERSION 3.1.0)

IF (CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

IF (MSVC)
    SET (CMAKE_CXX_FLAGS "/Wall")
ENDIF (MSVC)

INCLUDE_DIRECTORIES ("..")

IF (UNIX)
  LIST (APPEND LIBS "dl")
ENDIF (UNIX)

SET (SOURCES
  "control.h"
  "control.cc"
  "control-set.h"
  "control-set.cc"
  "driver.h"
  "driver.cc"
  "driver-manager.h"
  "driver-manager.cc"
  "effect.h"
  "effect.cc"
  "effect-list.h"
  "effect-list.cc"
  "exception.h"
  "exception.cc"
  "shared-library.h"
  "shared-library.cc"
  "shared-library-collection.h"
  "shared-library-collection.cc")

FIND_PACKAGE(ALSA)
if (ALSA_FOUND)
  LIST (APPEND SOURCES
    "alsa-driver.h"
    "alsa-driver.cc")
  ADD_DEFINITIONS (-DGNUITAR_WITH_ALSA=1)
  LIST (APPEND LIBS ${ALSA_LIBRARIES})
  LIST (APPEND LIBS "pthread")
ENDIF (ALSA_FOUND)

ADD_LIBRARY ("gnuitar" ${SOURCES})

IF (DEFINED LIBS)
  TARGET_LINK_LIBRARIES ("gnuitar" ${LIBS})
ENDIF (DEFINED LIBS)

ADD_EXECUTABLE ("driver-test" "driver-test.cc")
TARGET_LINK_LIBRARIES ("driver-test" "gnuitar")

ADD_EXECUTABLE ("driver-manager-test" "driver-manager-test.cc")
TARGET_LINK_LIBRARIES ("driver-manager-test" "gnuitar")

