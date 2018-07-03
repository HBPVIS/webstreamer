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

file(GLOB  NOISE_STREAM_SOURCES src/*.cpp)
file(GLOB  NOISE_STREAM_HEADERS src/*.hpp)

add_executable(noise-stream
  ${NOISE_STREAM_SOURCES}
  ${NOISE_STREAM_HEADERS}
)
add_test(NAME "cpplint@noise-stream" COMMAND "python" "${CMAKE_SOURCE_DIR}/cpplint.py"
  ${NOISE_STREAM_SOURCES}
  ${NOISE_STREAM_HEADERS}
)

SetWarningLevelRWTH(noise-stream)


# --- dependencies --
# webstreamer
target_include_directories(noise-stream PUBLIC webstreamer)
target_link_libraries(noise-stream webstreamer)
