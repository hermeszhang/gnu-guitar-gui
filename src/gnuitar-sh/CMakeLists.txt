CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF (MSVC)
    SET (CMAKE_CXX_FLAGS "/Wall")
ELSE (MSVC)
    SET (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++14")
ENDIF (MSVC)

ADD_EXECUTABLE(gnuitar-sh
    gnuitar-sh.cc
    shell.h
    shell.cc)

TARGET_LINK_LIBRARIES(gnuitar-sh gnuitar)

INSTALL(TARGETS gnuitar-sh
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

