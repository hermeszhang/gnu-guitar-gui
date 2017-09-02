cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "/Wall")
else(MSVC)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors")
endif(MSVC)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

set (INCDIR "${PROJECT_SOURCE_DIR}/include/gnu-guitar-qt")
set (SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable("gnu-guitar-qt"
  "${SRCDIR}/gnu-guitar-qt.cpp"
  "${SRCDIR}/audio-panel.cc"
  "${INCDIR}/audio-panel.h"
  "${SRCDIR}/controller.cpp"
  "${INCDIR}/controller.hpp"
  "${SRCDIR}/effect-view.cc"
  "${INCDIR}/effect-view.h"
  "${SRCDIR}/button.cc"
  "${INCDIR}/button.h"
  "${SRCDIR}/doc-browser.cc"
  "${INCDIR}/doc-browser.h"
  "${INCDIR}/ladspa-setup.hpp"
  "${SRCDIR}/ladspa-setup.cpp"
  "${SRCDIR}/knob.cc"
  "${INCDIR}/knob.h"
  "${SRCDIR}/main-window.cpp"
  "${INCDIR}/main-window.hpp"
  "${SRCDIR}/menu-bar.cc"
  "${INCDIR}/menu-bar.h"
  "${SRCDIR}/rack.cc"
  "${INCDIR}/rack.h"
  "resources.qrc")

target_link_libraries("gnu-guitar-qt"
  "rtaudio"
  Qt5::Widgets)
#  ${Qt5Core_QTMAIN_LIBRARIES})

qt5_use_modules("gnu-guitar-qt" Widgets)

install(TARGETS "gnu-guitar-qt"
  RUNTIME DESTINATION bin)
