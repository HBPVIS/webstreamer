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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_FLAGS_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_FLAGS_HPP_

#include <type_traits>

namespace webstreamer {

#define WEBSTREAMER_CONVERT_TYPED_ENUM_TO_FLAGS(enum_name)            \
  inline enum_name operator|(enum_name lhs, enum_name rhs) {          \
    using UnderlyingType = std::underlying_type_t<enum_name>;         \
    return static_cast<enum_name>(static_cast<UnderlyingType>(lhs) |  \
                                  static_cast<UnderlyingType>(rhs));  \
  }                                                                   \
  inline enum_name operator^(enum_name lhs, enum_name rhs) {          \
    using UnderlyingType = std::underlying_type_t<enum_name>;         \
    return static_cast<enum_name>(static_cast<UnderlyingType>(lhs) ^  \
                                  static_cast<UnderlyingType>(rhs));  \
  }                                                                   \
  inline enum_name operator~(enum_name rhs) {                         \
    using UnderlyingType = std::underlying_type_t<enum_name>;         \
    return static_cast<enum_name>(~static_cast<UnderlyingType>(rhs)); \
  }                                                                   \
  inline bool CheckForFlags(enum_name flags_to_check,                 \
                            enum_name flags_to_check_for) {           \
    using UnderlyingType = std::underlying_type_t<enum_name>;         \
    const enum_name bitwise_and = static_cast<enum_name>(             \
        static_cast<UnderlyingType>(flags_to_check) &                 \
        static_cast<UnderlyingType>(flags_to_check_for));             \
    return bitwise_and == flags_to_check_for;                         \
  }
}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_FLAGS_HPP_
