CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (CMAKE_COMPILER_IS_GNUCC)
    SET (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

ADD_LIBRARY(gnuitar-builtins SHARED
    package.c
    echo.c
    echo.h)

