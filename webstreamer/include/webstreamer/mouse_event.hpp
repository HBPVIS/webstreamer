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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_MOUSE_EVENT_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_MOUSE_EVENT_HPP_

#include <cstdint>
#include "webstreamer/event.hpp"
#include "webstreamer/export.hpp"
#include "webstreamer/flags.hpp"

namespace webstreamer {

#ifdef DOUBLE_CLICK
#undef DOUBLE_CLICK
#endif
enum class MouseAction : std::uint8_t {
  MOVE = 0,
  BUTTON_DOWN = 1,
  BUTTON_UP = 2,
  DOUBLE_CLICK = 3
};

enum class MouseButton : std::uint8_t {
  MAIN = 0,
  AUXILIARY = 1,
  SECONDARY = 2,
  FOURTH = 3,
  FIFTH = 4
};

enum class MouseButtons : std::uint8_t {
  NONE = 0,
  PRIMARY = 1,
  SECONDARY = 2,
  AUXILIARY = 4,
  FOURTH = 8,
  FIFTH = 16
};
WEBSTREAMER_CONVERT_TYPED_ENUM_TO_FLAGS(MouseButtons)

class WEBSTREAMER_EXPORT MouseEvent : public Event {
 public:
  MouseEvent(const void* data, std::size_t size_in_bytes);

  std::vector<std::uint8_t> Serialize() const override;
  std::string ToString() const override;

  inline MouseAction action() const { return action_; }
  inline MouseButton button() const { return button_; }
  inline MouseButtons buttons() const { return buttons_; }
  inline double x() const { return x_; }
  inline double y() const { return y_; }

 private:
  double x_;
  double y_;
  MouseAction action_;
  MouseButton button_;
  MouseButtons buttons_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_MOUSE_EVENT_HPP_
