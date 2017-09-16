cmake_minimum_required(VERSION 2.6)

set (CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "/W3 /EHsc /std:c++latest")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++17")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-missing-braces -Werror -Wfatal-errors -std=c++1z")
endif(MSVC)

if (GNU_GUITAR_QT_WITH_CORE)
  add_definitions(-DGNU_GUITAR_QT_WITH_CORE=1)
endif (GNU_GUITAR_QT_WITH_CORE)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

macro (ADD_GUI_CLASS VAR HEADER SOURCE)
  list (APPEND ${VAR}
    "${PROJECT_SOURCE_DIR}/include/gnu-guitar/gui/${HEADER}"
    "${PROJECT_SOURCE_DIR}/src/${SOURCE}")
endmacro (ADD_GUI_CLASS VAR HEADER SOURCE)

macro (ADD_QT_CLASS VAR HEADER SOURCE)
  list (APPEND ${VAR}
    "${PROJECT_SOURCE_DIR}/include/gnu-guitar/qt/${HEADER}"
    "${PROJECT_SOURCE_DIR}/src/${SOURCE}")
endmacro (ADD_QT_CLASS VAR HEADER SOURCE)

unset (SOURCES)

add_gui_class(SOURCES
  "api-settings.hpp"
  "api-settings.cpp")

add_gui_class(SOURCES
  "control.hpp"
  "control.cpp")

add_gui_class(SOURCES
  "control-list.hpp"
  "control-list.cpp")

add_gui_class(SOURCES
  "control-visitor.hpp"
  "control-visitor.cpp")

add_gui_class(SOURCES
  "binary-control.hpp"
  "binary-control.cpp")

add_gui_class(SOURCES
  "string-control.hpp"
  "string-control.cpp")

add_qt_class(SOURCES
  "controller.hpp"
  "controller.cpp")

add_qt_class(SOURCES
  "combo-box.hpp"
  "combo-box.cpp")

add_qt_class(SOURCES
  "labelled-switch.hpp"
  "labelled-switch.cpp")

add_qt_class(SOURCES
  "form.hpp"
  "form.cpp")

add_gui_class(SOURCES
  "driver.hpp"
  "driver.cpp")

add_gui_class(SOURCES
  "fake-driver.hpp"
  "fake-driver.cpp")

if (GNU_GUITAR_QT_WITH_CORE)
  add_gui_class(SOURCES
    "core-driver.hpp"
    "core-driver.cpp")
endif (GNU_GUITAR_QT_WITH_CORE)

# Old variables, don't use them.
# Use add_gui_class or add_qt_class instead.
set (INCDIR "${PROJECT_SOURCE_DIR}/include/gnu-guitar-qt")
set (SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")

list (APPEND SOURCES
  "${SRCDIR}/gnu-guitar-qt.cpp"
  "${INCDIR}/api-preferences.hpp"
  "${SRCDIR}/api-preferences.cpp"
  "${INCDIR}/audio-panel.h"
  "${SRCDIR}/audio-panel.cc"
  "${INCDIR}/driver-preferences.hpp"
  "${SRCDIR}/driver-preferences.cpp"
  "${SRCDIR}/doc-browser.cc"
  "${INCDIR}/doc-browser.h"
  "${SRCDIR}/effect-view.cc"
  "${INCDIR}/effect-view.hpp"
  "${INCDIR}/ladspa-setup.hpp"
  "${SRCDIR}/ladspa-setup.cpp"
  "${SRCDIR}/knob.cc"
  "${INCDIR}/knob.h"
  "${INCDIR}/main-window.hpp"
  "${SRCDIR}/main-window.cpp"
  "${INCDIR}/menu-bar.h"
  "${SRCDIR}/menu-bar.cc"
  "${INCDIR}/power-button.hpp"
  "${SRCDIR}/power-button.cpp"
  "${INCDIR}/preferences.hpp"
  "${SRCDIR}/preferences.cpp"
  "${INCDIR}/rack.h"
  "${SRCDIR}/rack.cc"
  "resources.qrc")

add_executable("gnu-guitar-gui" ${SOURCES})

target_link_libraries("gnu-guitar-gui" Qt5::Widgets)
target_link_libraries("gnu-guitar-gui" "qaccordion")

if (GNU_GUITAR_QT_WITH_CORE)
  target_link_libraries("gnu-guitar-gui" "gnu-guitar-core")
endif (GNU_GUITAR_QT_WITH_CORE)

qt5_use_modules("gnu-guitar-gui" Widgets)

install(TARGETS "gnu-guitar-gui"
  RUNTIME DESTINATION bin)

