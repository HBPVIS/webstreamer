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

file(GLOB  CLI_RENDERER_SOURCES src/*.cpp)
file(GLOB  CLI_RENDERER_HEADERS src/*.hpp)

add_executable(test-stream
  ${CLI_RENDERER_SOURCES}
  ${CLI_RENDERER_HEADERS}
)
add_test(NAME "cpplint@test-stream" COMMAND "python" "${CMAKE_SOURCE_DIR}/cpplint.py"
  ${CLI_RENDERER_SOURCES}
  ${CLI_RENDERER_HEADERS}
)

set_warning_levels_rwth(test-stream)


# --- dependencies --
# webstreamer
target_include_directories(test-stream PUBLIC webstreamer)
target_link_libraries(test-stream webstreamer)

# libPNG
# target_include_directories(test-stream PUBLIC CONAN_PKG::libpng)
target_link_libraries(test-stream ${CONAN_LIBS})
# find_package(PNG REQUIRED)

# target_include_directories(test-stream PUBLIC ${PNG_INCLUDE_DIRS})
# target_link_libraries(test-stream ${PNG_LIBRARIES})
# add_definitions(${PNG_DEFINITIONS})
