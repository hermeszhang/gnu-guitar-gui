CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (CMAKE_COMPILER_IS_GNUCC)
    SET (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

IF (MSVC)
    SET (CMAKE_C_FLAGS "/Wall")
ENDIF (MSVC)

ADD_LIBRARY(gnuitar-builtins SHARED
    biquad.h
    biquad.c
    package.cc
    tremolo.h
    tremolo.c
    vibrato.h
    vibrato.c
    echo.h
    echo.c)

TARGET_LINK_LIBRARIES(gnuitar-builtins gnuitar)

IF (MSVC)
    SET_TARGET_PROPERTIES(gnuitar-builtins PROPERTIES
        LINK_FLAGS "/def:${CMAKE_CURRENT_SOURCE_DIR}/package.def")
ENDIF (MSVC)

