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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_KEYBOARD_EVENT_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_KEYBOARD_EVENT_HPP_

#include <cstdint>
#include "webstreamer/event.hpp"
#include "webstreamer/export.hpp"
#include "webstreamer/flags.hpp"

namespace webstreamer {

enum class KeyboardAction : std::uint8_t {
  KEY_DOWN = 0,
  KEY_PRESS = 1,
  KEY_UP = 2,
};

class WEBSTREAMER_EXPORT KeyboardEvent : public Event {
 public:
  KeyboardEvent(const void* data, std::size_t size_in_bytes);

  std::vector<std::uint8_t> Serialize() const override;
  std::string ToString() const override;

  inline KeyboardAction action() const { return action_; }
  inline const std::string& code() const { return code_; }
  inline const std::string& key() const { return key_; }

 private:
  KeyboardAction action_;
  std::string code_;
  std::string key_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_MOUSE_EVENT_HPP_
