find_path(RTAUDIO_INCLUDE_DIR "rtaudio/rtaudio.hpp"
	HINTS "${CMAKE_INSTALL_PREFIX}/include")

message(${CMAKE_INSTALL_PREFIX}/include)
message(${RTAUDIO_INCLUDE_DIR})

find_library(RTAUDIO_LIBRARY "rtaudio")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(RtAudio DEFAULT_MSG
	RTAUDIO_LIBRARY
	RTAUDIO_INCLUDE_DIR)

set(RTAUDIO_LIBRARIES ${RTAUDIO_LIBRARY})
set (RTAUDIO_INCLUDE_DIRS ${RTAUDIO_INCLUDE_DIR})
