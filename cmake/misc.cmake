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

# Sets C++ standard version and disables extensions
# Synopsis: use_cxx_standard(version)
function(use_cxx_standard version)
    set(CMAKE_CXX_STANDARD ${version} PARENT_SCOPE)
    set(CMAKE_CXX_EXTENSIONS OFF PARENT_SCOPE)
    set(CMAKE_CXX_STANDARD_REQUIRED ON PARENT_SCOPE)
    message(STATUS "Using C++${version}")

    if(${CMAKE_VERSION} LESS 3.2)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
            include(CheckCXXCompilerFlag)
            check_cxx_compiler_flag(--std=c++${CMAKE_CXX_STANDARD} STD_CXX)
            if(STD_CXX)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++${CMAKE_CXX_STANDARD}" PARENT_SCOPE)
            else()
                message(SEND_ERROR "Requires C++${CMAKE_CXX_STANDARD} or better")
            endif()
        else()
            message(WARNING "Unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}, make sure it supports C++${CMAKE_CXX_STANDARD}")
        endif()
    endif()
endfunction()

# Searches all sources matching a some glob patterns
# Synopsis: find_sources(output_variable pattern...)
# Make sure to populate ALL_SOURCE_DIRECTORIES before calling this
function(find_sources output_var)
    set(all_files)
    foreach(pattern ${ARGN})
        file(GLOB_RECURSE files "${ALL_SOURCE_DIRECTORIES}/${pattern}")
        list(APPEND all_files ${files})
    endforeach()
    set(${output_var} ${all_files} PARENT_SCOPE)
endfunction()


# Creates targets to generate Doxygen documentation
# Synopsis: create_doxygen_target(name)
function(create_doxygen_target name)
    set(DOXYGEN_OUTPUT "\"${CMAKE_BINARY_DIR}/doc\"")

    set(DOXYGEN_INPUT "")
    find_sources(DOXYGEN_INPUT_FILES "*.dox")
    set(DOXYGEN_INPUT_FILES ${DOXYGEN_INPUT_FILES} ${ALL_SOURCES})
    foreach(source ${DOXYGEN_INPUT_FILES})
        set(DOXYGEN_INPUT "${DOXYGEN_INPUT} \"${source}\"")
    endforeach()

    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile)
    add_custom_target(${name}
        ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
        DEPENDS ${PROJECT_BINARY_DIR}/Doxyfile ${DOXYGEN_INPUT_FILES}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Generating Doxygen Documentation" VERBATIM
    )
    add_custom_target(${name}-view
        xdg-open ${DOXYGEN_OUTPUT}/html/index.html
        COMMENT "Showing Doxygen documentation"
    )
endfunction()
