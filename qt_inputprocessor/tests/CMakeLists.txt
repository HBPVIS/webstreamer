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

file(GLOB  QT_INPUTPROCESSOR_TEST_SOURCES src/*.cpp)
file(GLOB  QT_INPUTPROCESSOR_TEST_HEADERS src/*.hpp)

add_executable(qt_inputprocessor_tests
  ${QT_INPUTPROCESSOR_TEST_SOURCES} 
  ${QT_INPUTPROCESSOR_TEST_HEADERS}
  )
set(QT_INPUTPROCESSOR_TESTS_ARGUMENTS ""
  CACHE STRING
  "Arguments, e.g., tags, that are passed to vn_tests executable.")
add_test(NAME qt_inputprocessor_tests COMMAND qt_inputprocessor_tests ${QT_INPUTPROCESSOR_TESTS_ARGUMENTS})
add_test(NAME "cpplint@qt_inputprocessor/tests" COMMAND "python" "${CMAKE_SOURCE_DIR}/cpplint.py"
  ${QT_INPUTPROCESSOR_TEST_SOURCES}
  ${QT_INPUTPROCESSOR_TEST_HEADERS}
)

set_warning_levels_rwth(qt_inputprocessor_tests)


# --- dependencies ---
# catch
target_include_directories(qt_inputprocessor_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# webstreamer
target_include_directories(qt_inputprocessor_tests PUBLIC webstreamer)
target_link_libraries(qt_inputprocessor_tests webstreamer)
