CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (MSVC)
    SET (CMAKE_CXX_FLAGS "/Wall")
ELSE (MSVC)
    SET (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++14")
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

