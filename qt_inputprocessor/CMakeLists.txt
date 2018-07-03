#-------------------------------------------------------------------------------
# web streamer
#
# Copyright (c) 2017 RWTH Aachen University, Germany,
# Virtual Reality & Immersive Visualization Group.
#-------------------------------------------------------------------------------
#                                 License
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
#-------------------------------------------------------------------------------

file(GLOB QT_INPUT_PROCESSOR_SOURCES src/*.cpp)
file(GLOB QT_INPUT_PROCESSOR_HEADERS src/*.hpp)
file(GLOB QT_INPUT_PROCESSOR_API_HEADERS include/qt_inputprocessor/*.hpp)

set(QT_INPUT_PROCESSOR_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  CACHE PATH "Path to public headers in webstreamer's source tree."
)

add_library(qt_inputprocessor
  ${QT_INPUT_PROCESSOR_SOURCES}
  ${QT_INPUT_PROCESSOR_HEADERS}
  ${QT_INPUT_PROCESSOR_API_HEADERS}
)
add_test(NAME "cpplint@qt_inputprocessor" COMMAND "python" "${CMAKE_SOURCE_DIR}/cpplint.py"
  ${QT_INPUT_PROCESSOR_SOURCES}
  ${QT_INPUT_PROCESSOR_HEADERS}
  ${QT_INPUT_PROCESSOR_API_HEADERS}
)

GENERATE_EXPORT_HEADER(qt_inputprocessor EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/include/qt_inputprocessor/export.hpp")

target_include_directories(qt_inputprocessor PUBLIC ${QT_INPUT_PROCESSOR_INCLUDE_DIRS})

SetWarningLevelRWTH(qt_inputprocessor)


# --- dependencies ---

# webstreamer
target_include_directories(qt_inputprocessor PUBLIC webstreamer)
target_link_libraries(qt_inputprocessor webstreamer)

# Qt
if (DEFINED ENV{QT5_DIR})
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_DIR})
  include_directories(BEFORE $ENV{QT5_DIR}/include)
endif()
find_package(Qt5Core    REQUIRED)
find_package(Qt5Widgets REQUIRED)

target_link_libraries(qt_inputprocessor
  ${OPENGL_LIBRARIES}
  Qt5::Core
  Qt5::Widgets
)

# --- tests --
add_subdirectory(tests)

set(CONF_INCLUDE_DIRS "${CURRENT_SOURCE_DIR}/include")
configure_file(cmake/qt_inputprocessor-config.cmake.in
  "${PROJECT_BINARY_DIR}/qt_inputprocessor-config.cmake" @ONLY)
export(TARGETS qt_inputprocessor
  APPEND FILE "${PROJECT_BINARY_DIR}/webstreamer-targets.cmake")
