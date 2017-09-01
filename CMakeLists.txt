cmake_minimum_required(VERSION 3.1.0)

project("gnu-guitar-qt")

find_package(RtAudio REQUIRED)

include_directories(${RTAUDIO_INCLUDE_DIRS})
include_directories("include")

add_subdirectory("src")
