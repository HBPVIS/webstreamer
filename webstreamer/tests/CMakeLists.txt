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

file(GLOB  VN_TEST_SOURCES src/*.cpp)
file(GLOB  VN_TEST_HEADERS src/*.hpp)

add_executable(vn_tests
  ${VN_TEST_SOURCES} 
  ${VN_TEST_SOURCES}
  )
set(VN_TESTS_ARGUMENTS ""
  CACHE STRING
  "Arguments, e.g., tags, that are passed to vn_tests executable.")
add_test(NAME vn_tests COMMAND vn_tests ${VN_TESTS_ARGUMENTS})
add_test(NAME "cpplint@vn/tests" COMMAND "python" "${CMAKE_SOURCE_DIR}/cpplint.py"
  ${VN_TEST_SOURCES}
  ${VN_TEST_HEADERS}
)

SetWarningLevelRWTH(vn_tests)


# --- dependencies ---
# catch
target_include_directories(vn_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# vn
target_include_directories(vn_tests PUBLIC vn)
target_link_libraries(vn_tests webstreamer)
