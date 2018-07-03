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

file(GLOB  LOOP_STREAM_SOURCES src/*.cpp)
file(GLOB  LOOP_STREAM_HEADERS src/*.hpp)

add_executable(loop-stream
  ${LOOP_STREAM_SOURCES}
  ${LOOP_STREAM_HEADERS}
)
add_test(NAME "cpplint@loop-stream" COMMAND "python" "${CMAKE_SOURCE_DIR}/cpplint.py"
  ${LOOP_STREAM_SOURCES}
  ${LOOP_STREAM_HEADERS}
)

SetWarningLevelRWTH(loop-stream)


# --- dependencies --
# webstreamer
target_include_directories(loop-stream PUBLIC webstreamer)
target_link_libraries(loop-stream webstreamer)
# target_compile_definitions(loop-stream PUBLIC webstreamer)

# libPNG
# target_include_directories(loop-stream PUBLIC CONAN_PKG::libpng)
target_link_libraries(loop-stream ${CONAN_LIBS})
# find_package(PNG REQUIRED)

# target_include_directories(loop-stream PUBLIC ${PNG_INCLUDE_DIRS})
# target_link_libraries(loop-stream ${PNG_LIBRARIES})
# add_definitions(${PNG_DEFINITIONS})
