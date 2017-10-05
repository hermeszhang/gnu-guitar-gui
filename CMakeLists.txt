cmake_minimum_required(VERSION 3.1.0)

project("GNU Guitar GUI")

set (GNU_GUITAR_GUI_VERSION_MAJOR 0)
set (GNU_GUITAR_GUI_VERSION_MINOR 4)
set (GNU_GUITAR_GUI_VERSION_PATCH 0)
set (GNU_GUITAR_GUI_VERSION "0.4.0")

include_directories("include")

add_subdirectory("external")
add_subdirectory("src")

set (CPACK_PACKAGE_NAME "GNU Guitar GUI")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A graphical interface for the GNU Guitar project.")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_VERSION_MAJOR ${GNU_GUITAR_GUI_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${GNU_GUITAR_GUI_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${GNU_GUITAR_GUI_VERSION_PATCH})
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")
include(CPack)
