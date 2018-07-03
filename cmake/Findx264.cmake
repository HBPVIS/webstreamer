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

# - Try to find x264
# Once done this will define
#  X264_FOUND - System has x264
#  X264_INCLUDE_DIRS - The x264 include directories
#  X264_LIBRARIES - The libraries needed to use x264
#  X264_DEFINITIONS - Compiler switches required for using x264

find_package(PkgConfig)
pkg_check_modules(PC_X264 QUIET x264)
set(X264_DEFINITIONS ${PC_X264_CFLAGS_OTHER})

find_path(X264_INCLUDE_DIR x264.h
          HINTS ${PC_X264_INCLUDEDIR} ${PC_X264_INCLUDE_DIRS} )

find_library(X264_LIBRARY NAMES x264
             HINTS ${PC_X264__LIBDIR} ${PC_X264_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set X264_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(x264  DEFAULT_MSG
                                  X264_LIBRARY X264_INCLUDE_DIR)

mark_as_advanced(X264_INCLUDE_DIR X264_LIBRARY )

set(X264_LIBRARIES ${X264_LIBRARY} )
set(X264_INCLUDE_DIRS ${X264_INCLUDE_DIR} )
