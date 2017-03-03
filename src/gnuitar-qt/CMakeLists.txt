CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++11")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

SET (CMAKE_AUTOMOC ON)
SET (CMAKE_AUTOUIC ON)
SET (CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)

QT5_WRAP_UI(UI_HEADERS main-window.ui)

ADD_EXECUTABLE(gnuitar-qt
    gnuitar-qt.cc
    ${UI_HEADERS}
    doc-browser.h
    doc-browser.cc
    knob.h
    knob.cc
    main-window.h
    main-window.cc
    rack.h
    rack.cc)
TARGET_LINK_LIBRARIES(gnuitar-qt ${Qt5Core_QTMAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(gnuitar-qt gnuitar)
QT5_USE_MODULES(gnuitar-qt Widgets)

ADD_EXECUTABLE(doc-browser-test doc-browser-test.cc doc-browser.cc)
TARGET_LINK_LIBRARIES(doc-browser-test ${Qt5Core_QTMAIN_LIBRARIES})
QT5_USE_MODULES(doc-browser-test Widgets)

INSTALL(TARGETS gnuitar-qt
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

