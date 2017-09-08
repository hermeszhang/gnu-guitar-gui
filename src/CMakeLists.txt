cmake_minimum_required(VERSION 2.6)

set (CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "/W3 /EHsc /std:c++latest")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++17")
else("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
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

set (INCDIR "${PROJECT_SOURCE_DIR}/include/gnu-guitar-qt")
set (SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")

set (SOURCES
  "${SRCDIR}/gnu-guitar-qt.cpp"
  "${SRCDIR}/api-preferences-list.cpp"
  "${INCDIR}/api-preferences-list.hpp"
  "${SRCDIR}/audio-panel.cc"
  "${INCDIR}/audio-panel.h"
  "${SRCDIR}/button.cc"
  "${INCDIR}/button.h"
  "${SRCDIR}/controller.cpp"
  "${INCDIR}/controller.hpp"
  "${INCDIR}/driver.hpp"
  "${SRCDIR}/driver.cpp"
  "${SRCDIR}/doc-browser.cc"
  "${INCDIR}/doc-browser.h"
  "${SRCDIR}/effect-view.cc"
  "${INCDIR}/effect-view.hpp"
  "${INCDIR}/fake-driver.hpp"
  "${SRCDIR}/fake-driver.cpp"
  "${INCDIR}/ladspa-setup.hpp"
  "${SRCDIR}/ladspa-setup.cpp"
  "${SRCDIR}/knob.cc"
  "${INCDIR}/knob.h"
  "${SRCDIR}/main-window.cpp"
  "${INCDIR}/main-window.hpp"
  "${SRCDIR}/menu-bar.cc"
  "${INCDIR}/menu-bar.h"
  "${SRCDIR}/power-button.cpp"
  "${INCDIR}/power-button.hpp"
  "${SRCDIR}/preferences.cpp"
  "${INCDIR}/preferences.hpp"
  "${SRCDIR}/rack.cc"
  "${INCDIR}/rack.h"
  "resources.qrc")

if (GNU_GUITAR_QT_WITH_CORE)
  list (APPEND SOURCES
    "${SRCDIR}/core-driver.cpp"
    "${INCDIR}/core-driver.hpp")
endif (GNU_GUITAR_QT_WITH_CORE)

add_executable("gnu-guitar-qt" ${SOURCES})

target_link_libraries("gnu-guitar-qt" Qt5::Widgets)

if (GNU_GUITAR_QT_WITH_CORE)
  target_link_libraries("gnu-guitar-qt" "gnu-guitar-core")
endif (GNU_GUITAR_QT_WITH_CORE)

qt5_use_modules("gnu-guitar-qt" Widgets)

install(TARGETS "gnu-guitar-qt"
  RUNTIME DESTINATION bin)

