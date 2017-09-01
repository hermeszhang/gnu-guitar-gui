CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE_DIRECTORIES ("..")

SET (CMAKE_CXX_STANDARD 11)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_CXX_EXTENSIONS OFF)

IF (MSVC)
    SET (CMAKE_CXX_FLAGS "/Wall")
ELSE (MSVC)
    SET (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
ENDIF (MSVC)

SET (CMAKE_AUTORCC ON)
SET (CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
SET (CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

ADD_EXECUTABLE(gnuitar-qt
    gnuitar-qt.cc
    audio-panel.h
    audio-panel.cc
    controller.h
    controller.cc
    effect-view.h
    effect-view.cc
    button.h
    button.cc
    doc-browser.h
    doc-browser.cc
    knob.h
    knob.cc
    main-window.h
    main-window.cc
    menu-bar.h
    menu-bar.cc
    rack.h
    rack.cc
    resources.qrc)
TARGET_LINK_LIBRARIES(gnuitar-qt ${Qt5Core_QTMAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(gnuitar-qt ${RTAUDIO_LIBRARIES})
QT5_USE_MODULES(gnuitar-qt Widgets)

INSTALL(TARGETS gnuitar-qt
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

FUNCTION(ADD_GNUITAR_QT_TEST TEST_NAME)
    ADD_EXECUTABLE(${TEST_NAME} ${ARGN})
    TARGET_LINK_LIBRARIES(${TEST_NAME} ${Qt5Core_QTMAIN_LIBRARIES})
    QT5_USE_MODULES(${TEST_NAME} Widgets)
ENDFUNCTION(ADD_GNUITAR_QT_TEST TEST_NAME)

ADD_GNUITAR_QT_TEST(audio-panel-test
    audio-panel-test.cc
    audio-panel.cc
    button.cc
    resources.qrc)

ADD_GNUITAR_QT_TEST(menu-bar-test
    menu-bar-test.cc
    menu-bar.cc
    doc-browser.cc
    resources.qrc)

ADD_GNUITAR_QT_TEST(doc-browser-test
    doc-browser-test.cc
    doc-browser.cc
    resources.qrc)

