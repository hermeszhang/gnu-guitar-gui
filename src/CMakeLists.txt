CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

SET(SOURCES
    chain.h
    chain.c
    effect.h
    effect.c
    map.c
    map.h
    packet.h
    packet.c
    pipe.h
    pipe.c
    track.h
    track.c
    utils.h
    utils.c)

FIND_PACKAGE(ALSA)
IF (ALSA_FOUND)
    ADD_DEFINITIONS(-DGNUITAR_WITH_ALSA)
    LIST (APPEND SOURCES
        track-alsa.h
        track-alsa.c)
ENDIF (ALSA_FOUND)

FIND_PACKAGE(Threads REQUIRED)

ADD_LIBRARY(gnuitar ${SOURCES})

TARGET_LINK_LIBRARIES(gnuitar ${ALSA_LIBRARIES})
TARGET_LINK_LIBRARIES(gnuitar ${CMAKE_THREAD_LIBS_INIT})

ADD_SUBDIRECTORY(gnuitar-sh)

