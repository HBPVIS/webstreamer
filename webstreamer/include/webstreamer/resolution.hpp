//------------------------------------------------------------------------------
// Web Streamer
//
// Copyright (c) 2017 RWTH Aachen University, Germany,
// Virtual Reality & Immersive Visualization Group.
//------------------------------------------------------------------------------
//                                 License
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_RESOLUTION_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_RESOLUTION_HPP_

#include <cstddef>

namespace webstreamer {

struct Resolution {
  int width;
  int height;
  int bitrate;
  int fps;
};

inline bool operator==(const Resolution& lhs, const Resolution& rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

inline bool operator!=(const Resolution& lhs, const Resolution& rhs) {
  return lhs.width != rhs.width || lhs.height != rhs.height;
}

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_RESOLUTION_HPP_
