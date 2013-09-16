#   This file is part of ini2json.
#
#   ini2json is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   ini2json is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with ini2json.  If not, see <http://www.gnu.org/licenses/>.

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ini2json_SRC
    Convert/Converter.cpp
    Convert/Converter.h
    Ini/IniFile.cpp
    Ini/IniFile.h
    Main/Main.cpp
    Parse/ParsedFile.cpp
    Parse/ParsedFile.h
    Parse/ParsedLine.cpp
    Parse/ParsedLine.h
)

add_executable(ini2json ${ini2json_SRC})

if(WIN32)
    # copies the README and COPYING files to the output directory (build/bin)
    add_custom_command(TARGET ini2json POST_BUILD COMMAND
                       ${CMAKE_COMMAND} -E copy
                       ${PROJECT_SOURCE_DIR}/README
                       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)/)
    add_custom_command(TARGET ini2json POST_BUILD COMMAND
                       ${CMAKE_COMMAND} -E copy
                       ${PROJECT_SOURCE_DIR}/COPYING
                       ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(ConfigurationName)/)
endif()
