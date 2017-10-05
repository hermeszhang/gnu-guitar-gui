# CMake wrapper to call windeployqt in Windows
#
# The files are placed in windeployqt folder in the toplevel
# build directory
#
# Taken from Neovim Qt

function(WinDeployQt)

  cmake_parse_arguments(windeploy
    "COMPILER_RUNTIME;FORCE"
    "TARGET"
    "INCLUDE_MODULES;EXCLUDE_MODULES"
    ${ARGN})

  if(NOT windeploy_TARGET)
    message(FATAL_ERROR "A TARGET must be specified")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND _ARGS --debug)
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    list(APPEND _ARGS --release-with-debug-info)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND _ARGS --release)
  endif()
  if(windeploy_COMPILER_RUNTIME)
    list(APPEND _ARGS --compiler-runtime)
  endif()
  if(windeploy_FORCE)
    list(APPEND _ARGS --force)
  endif()

  foreach(mod ${windeploy_INCLUDE_MODULES})
    string(TOLOWER ${mod} mod)
    string(REPLACE "qt5::" "" mod ${mod})
    list(APPEND _ARGS "--${mod}")
  endforeach()
  foreach(mod ${windeploy_EXCLUDE_MODULES})
    string(TOLOWER ${mod} mod)
    string(REPLACE "qt5::" "" mod ${mod})
    list(APPEND _ARGS "--no-${mod}")
  endforeach()

  find_program(windeploy_PROGRAM windeployqt
    PATHS $ENV{QTDIR}/bin/)
  if(windeploy_PROGRAM)
    message(STATUS "Found ${windeploy_PROGRAM}")
  else()
    message(FATAL_ERROR "Unable to find windeployqt")
  endif()

  if(COMPILER_RUNTIME AND NOT $ENV{VVVV})
    message(STATUS "not set, the VC++ redistributable installer will NOT be bundled")
  endif()

  add_custom_target(windeployqt ALL ${windeploy_PROGRAM} --dir ${PROJECT_BINARY_DIR}/windeployqt ${_ARGS}
    $<TARGET_FILE:${windeploy_TARGET}>
    DEPENDS ${windeploy_TARGET}
    COMMENT "Preparing Qt runtime dependencies")
endfunction()
