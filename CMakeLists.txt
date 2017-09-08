cmake_minimum_required(VERSION 3.1.0)

project("gnu-guitar-qt")

include_directories("include")

if (GNU_GUITAR_QT_WITH_CORE)
  find_package(RtAudio REQUIRED)
  include_directories(${RTAUDIO_INCLUDE_DIRS})
endif (GNU_GUITAR_QT_WITH_CORE)

add_subdirectory("src")
