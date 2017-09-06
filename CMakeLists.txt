cmake_minimum_required(VERSION 3.1.0)

project("gnu-guitar-qt")

include_directories("include")

find_package(RtAudio)
if (RtAudio_FOUND)
  include_directories(${RTAUDIO_INCLUDE_DIRS})
endif (RtAudio_FOUND)

add_subdirectory("src")
