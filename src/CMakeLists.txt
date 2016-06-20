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
set(EXECUTABLE_NAME ascedit) # repeated otherwise kdevelop messes up

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES
main.cpp
color/cute_color.cpp
)


# Enable extra Qt tools
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#if ( CMAKE_MAJOR_VERSION LESS 3 )
set(UI_FILES
)
qt5_wrap_ui(SOURCES ${UI_FILES})
#endif()

# Executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Qt
target_link_libraries(${EXECUTABLE_NAME} Qt5::Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Install
install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)
