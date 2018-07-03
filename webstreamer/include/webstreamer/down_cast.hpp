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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_DOWN_CAST_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_DOWN_CAST_HPP_

#include <type_traits>

namespace webstreamer {

// Performs a checked down cast via dynamic_cast in debug mode and just uses
// a static_cast in release mode
template <typename DerivedType, typename BaseType>
inline DerivedType down_cast(BaseType&& base) {
  static_assert(std::is_pointer<DerivedType>::value ||
                    std::is_reference<DerivedType>::value,
                "DerivedType must be a pointer or a reference");
  static_assert(
      std::is_pointer<BaseType>::value || std::is_reference<BaseType>::value,
      "BaseType must be a pointer or a reference");
  static_assert(
      std::is_base_of<
          std::remove_pointer_t<std::remove_reference_t<BaseType>>,
          std::remove_pointer_t<std::remove_reference_t<DerivedType>>>::value,
      "BaseType must be a base of DerivedType");
#if NDEBUG
  return static_cast<DerivedType>(base);
#else
  return dynamic_cast<DerivedType>(base);
#endif
}

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_DOWN_CAST_HPP_
