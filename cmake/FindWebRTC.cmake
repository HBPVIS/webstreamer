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

# - Try to find WebRTC
# Once done this will define
#  WEBRTC_FOUND - System has WebRTC
#  WEBRTC_INCLUDE_DIRS - The WebRTC include directories
#  WEBRTC_LIBRARIES - The libraries needed to use WebRTC
#  WEBRTC_DEFINITIONS - Compiler switches required for using WebRTC

find_package(PkgConfig)
pkg_search_module(PC_WEBRTC libwebrtc libwebrtc_full)

find_path(WEBRTC_INCLUDE_DIR webrtc/pc/peerconnection.h
          HINTS ${PC_WEBRTC_INCLUDEDIR} ${PC_WEBRTC_INCLUDE_DIRS} ${WEBRTC_DIR}/include)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
  set(LIBPATH_SUFFIX "x64/Debug")
else ()
  set(LIBPATH_SUFFIX "x64/Release")
endif ()

find_library(WEBRTC_LIBRARY NAMES webrtc libwebrtc webrtc_full libwebrtc_full
             HINTS ${PC_WEBRTC_LIBDIR} ${PC_WEBRTC_LIBRARY_DIRS} ${WEBRTC_DIR}/lib
             PATH_SUFFIXES ${LIBPATH_SUFFIX})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set WEBRTC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(WebRTC  DEFAULT_MSG
                                  WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR)

mark_as_advanced(WEBRTC_INCLUDE_DIR WEBRTC_LIBRARY)

if (${UNIX})
set(WEBRTC_LIBRARIES ${WEBRTC_LIBRARY} pthread dl)
elseif (${WIN32})
set(WEBRTC_LIBRARIES ${WEBRTC_LIBRARY} winmm Strmiids dmoguids wmcodecdspuuid amstrmid msdmo crypt32 iphlpapi secur32)
endif ()
set(WEBRTC_INCLUDE_DIRS ${WEBRTC_INCLUDE_DIR} ${PC_WEBRTC_INCLUDEDIR} ${PC_WEBRTC_INCLUDE_DIRS} )
set(WEBRTC_DEFINITIONS ${PC_WEBRTC_CFLAGS})
