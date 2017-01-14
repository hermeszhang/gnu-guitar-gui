CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

ADD_DEFINITIONS(-DFLOAT_DSP)

find_package(GTK2 REQUIRED)

INCLUDE_DIRECTORIES("${GTK2_INCLUDE_DIRS}")
ADD_DEFINITIONS(-DHAVE_GTK2)

IF (UNIX)
    # for vasprintf
    ADD_DEFINITIONS(-D_GNU_SOURCE)

    # OSS is always available on Unix
    ADD_DEFINITIONS(-DHAVE_OSS)

    # these libraries are dynamically detected.
    # probably pkg-config should be used for these as well.
    FIND_LIBRARY(JACK_LIBRARY NAMES jack)
    IF (JACK_LIBRARY)
        ADD_DEFINITIONS(-DHAVE_JACK)
        SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} jack)
    ELSE (JACK_LIBRARY)
        MESSAGE("*** Warning: libjack not found -- not compiling jack support.")
    ENDIF (JACK_LIBRARY)

    FIND_LIBRARY(ALSA_LIBRARY NAMES asound)
    IF (ALSA_LIBRARY)
        ADD_DEFINITIONS(-DHAVE_ALSA)
        SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} asound)
    ELSE (ALSA_LIBRARY)
        MESSAGE("*** Warning: libasound not found -- not compiling ALSA support.")
    ENDIF (ALSA_LIBRARY)

    # sndfile is only required for unix
    FIND_LIBRARY(SNDFILE_LIBRARY NAMES sndfile)
    IF (SNDFILE_LIBRARY)
        ADD_DEFINITIONS(-DHAVE_SNDFILE)
        SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} sndfile)
    ELSE (SNDFILE_LIBRARY)
        MESSAGE("*** Warning: libsndfile not found -- .wav export not possible.")
    ENDIF (SNDFILE_LIBRARY)

    SET(THREADS_PREFER_PTHREAD_FLAG ON)
    FIND_PACKAGE(Threads REQUIRED)
    SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

    SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} dl m)

ENDIF (UNIX)

IF (WIN32)
    FIND_LIBRARY(DSOUND_LIBRARY NAMES dsound)
    IF (DSOUND_LIBRARY)
        ADD_DEFINITIONS(-DHAVE_DSOUND)
        SET (OTHER_LIBRARIES ${OTHER_LIBRARIES} dsound dxguid)
    ELSE (DSOUND_LIBRARY)
        MESSAGE("*** Warning: dsound not found -- disabling DirectSound output.")
    ENDIF (DSOUND_LIBRARY)

    FIND_LIBRARY(WINMM_LIBRARY NAMES winmm)
    IF (WINMM_LIBRARY)
        ADD_DEFINITIONS(-DHAVE_WINMM)
        SET (OTHER_LIBRARIES ${OTHER_LIBRARIES} winmm)
    ELSE (WINMM_LIBRARY)
        MESSAGE("*** Warning: winmm not found -- disabling MMSystem output.")
    ENDIF (WINMM_LIBRARY)

    IF (NOT DSOUND_LIBRARY AND NOT WINMM_LIBRARY)
        MESSAGE(FATAL_ERROR "*** Error: Neither DirectSound or WinMM libraries could be found for audio playback.")
    ENDIF (NOT DSOUND_LIBRARY AND NOT WINMM_LIBRARY)
ENDIF (WIN32)

FIND_LIBRARY(FFTW3_LIBRARY NAMES fftw3f)
IF (FFTW3_LIBRARY)
    ADD_DEFINITIONS(-DHAVE_FFTW3)
    SET(OTHER_LIBRARIES ${OTHER_LIBRARIES} fftw3f)
ELSE (FFTW3_LIBRARY)
    MESSAGE("*** Warning: libfftw3f not found -- using time-domain implementations where possible.")
ENDIF (FFTW3_LIBRARY)

# set final C flags
SET(CMAKE_C_FLAGS "${ARCH_FLAGS} ${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${OPT_FLAGS}")

SET(SOURCES
    amp.c
    package.c
    main.c
    pump.c
    chorus.c
    delay.c
    echo.c
    tracker.c
    tremolo.c
    vibrato.c
    autowah.c
    phasor.c
    rcfilter.c
    tubeamp.c
    rotary.c
    gui.c
    distort.c
    distort2.c
    sustain.c
    reverb.c
    backbuf.c
    utils.c
    noise.c
    biquad.c
    eqbank.c
    tuner.c
    audio-alsa.c
    audio-oss.c
    pitch.c
    glib12-compat.c
    audio-jack.c
    audio-winmm.c
    audio-dsound.c
    audio-driver.c
    audio-midi.c
    effect.c)

ADD_EXECUTABLE(gnuitar ${SOURCES})

SET_TARGET_PROPERTIES(gnuitar PROPERTIES LINK_FLAGS "-Wl,--as-needed")
TARGET_LINK_LIBRARIES(gnuitar ${GTK2_LIBRARIES} ${OTHER_LIBRARIES})

INSTALL(TARGETS gnuitar RUNTIME DESTINATION bin)
MAKE_DIRECTORY("${CMAKE_INSTALL_PREFIX}/share/doc/gnuitar/docs")
INSTALL(FILES AUTHORS NEWS README TODO FAQ COPYING ChangeLog
        DESTINATION share/doc/gnuitar)
INSTALL(FILES docs/devdocs.txt docs/faq.html docs/index.html docs/install.html
        docs/readme.html
        DESTINATION share/doc/gnuitar/docs)

