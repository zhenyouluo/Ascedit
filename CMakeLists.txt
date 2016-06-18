#
# Copyright (C) 2015-2016 Mattia Basaglia
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0009 NEW)
set(NICE_NAME Ascedit)
project(${NICE_NAME} CXX)
set(PROJECT_VERSION 0.1)
string(TOLOWER ${NICE_NAME} EXECUTABLE_NAME)
set(PROJECT_DESCRIPTION "ASCII art editor")
set(PROJECT_LONG_DESCRIPTION "A graphical ascii art editor")
set(MAINTAINER "Mattia Basaglia <mattia.basaglia@gmail.com>")
set(PROJECT_WEBSITE "https://github.com/mbasaglia/Ascedit")
set(PROJECT_VERSION_LONG ${PROJECT_VERSION})
set(ICON_PATH "")

include(cmake/misc.cmake)

# C++ Standard
use_cxx_standard(14)

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -Werror")

# Enable Debug by default, can be changed with -D CMAKE_BUILD_TYPE=Release
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE Debug)

    execute_process(COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(GIT_VERSION)
        set(PROJECT_VERSION_LONG "${PROJECT_VERSION}-git-${GIT_VERSION}")
        add_custom_target(version_git ALL
            COMMAND [ "`git rev-parse --short HEAD`" != "${GIT_VERSION}" ] && ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}
            VERBATIM
        )
    endif()
endif()

add_subdirectory(src bin)

# Tests
include(cmake/testing.cmake)
add_subdirectory(test)

# Find all sources for documentation and stuff
set(ALL_SOURCE_DIRECTORIES src)


find_sources(ALL_SOURCES *.cpp *.hpp)

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    create_doxygen_target(doc)
endif(DOXYGEN_FOUND)

# Translation
include(cmake/qt.cmake)
create_qt_linguist_translations(
    translations
    translations/it_IT.ts
)

# Package generation
find_program(DPKG_PROGRAM dpkg)
if(DPKG_PROGRAM)
    execute_process(
        COMMAND ${DPKG_PROGRAM} --print-architecture
        OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CPACK_PACKAGE_FILE_NAME "${EXECUTABLE_NAME}_${PROJECT_VERSION_LONG}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_ICON ${ICON_PATH})
#CPACK_CREATE_DESKTOP_LINKS
set(CPACK_PACKAGE_CONTACT ${MAINTAINER})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_LONG})

set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PROJECT_WEBSITE})
set(CPACK_DEBIAN_PACKAGE_SECTION graphics)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n ${PROJECT_LONG_DESCRIPTION}")

include(CPack)
