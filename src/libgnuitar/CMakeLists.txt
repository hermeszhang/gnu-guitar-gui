CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

IF (CMAKE_COMPILER_IS_GNUCC)
    SET (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

IF (MSVC)
    SET (CMAKE_C_FLAGS "/Wall")
ENDIF (MSVC)

SET(SOURCES
    chain.h
    chain.c
    conversion.h
    conversion.c
    driver.h
    driver.c
    effect.h
    effect.c
    map.c
    map.h
    package.h
    package.c
    package-manager.h
    package-manager.c
    packet.h
    packet.c
    pipe.h
    pipe.c
    track.h
    track.c
    mutex.h
    mutex.c)

ADD_LIBRARY(gnuitar SHARED ${SOURCES})

INSTALL(TARGETS gnuitar
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

ADD_SUBDIRECTORY(gnuitar-builtins)

IF (GNUITAR_SH)
    ADD_SUBDIRECTORY(gnuitar-sh)
ENDIF (GNUITAR_SH)

IF (GNUITAR_QT)
    ADD_SUBDIRECTORY(gnuitar-qt)
ENDIF (GNUITAR_QT)

