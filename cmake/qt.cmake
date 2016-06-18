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

# Creates a target to compile Qt linguist translations
# Synopsis: create_qt_linguist_translations(target ts_files...)
function(create_qt_linguist_translations target_name)
    find_package(Qt5LinguistTools QUIET)
    if(Qt5LinguistTools_FOUND)
        set(TRANSLATIONS ${ARGN})
        foreach(TRANSLATION ${TRANSLATIONS})
            set(TS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${TRANSLATION}")
            set_source_files_properties(${TS_FILE} PROPERTIES
                OUTPUT_LOCATION
                "${CMAKE_BINARY_DIR}/share/${EXECUTABLE_NAME}/translations")
        endforeach()
        find_sources(UI_FILES *.ui)
        set(TRANSLATABLE ${ALL_SOURCES} ${UI_FILES})
        qt5_create_translation(QM_FILES ${TRANSLATIONS} ${TRANSLATABLE})
        add_custom_target(${target_name} DEPENDS ${QM_FILES})
        # TODO: check installation
    endif()
endfunction()
