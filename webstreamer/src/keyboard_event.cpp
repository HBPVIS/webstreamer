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

#include "webstreamer/keyboard_event.hpp"
#include <cassert>
#include <stdexcept>

namespace webstreamer {

namespace {

struct KeyboardEventData {
  EventType event_type;
  KeyboardAction action;
  std::uint8_t code_length;
  std::uint8_t key_length;
};
static_assert(sizeof(KeyboardEventData) == 4, "Invalid padding");

}  // namespace

KeyboardEvent::KeyboardEvent(const void* data, std::size_t size_in_bytes)
    : Event(EventType::KEYBOARD_INPUT) {
  if (size_in_bytes < sizeof(KeyboardEventData)) {
    throw std::runtime_error("Invalid event size");
  }

  auto keyboard_event_data = reinterpret_cast<const KeyboardEventData*>(data);
  assert(keyboard_event_data->event_type == EventType::KEYBOARD_INPUT);

  if (size_in_bytes != sizeof(KeyboardEventData) +
                           keyboard_event_data->code_length +
                           keyboard_event_data->key_length) {
    throw std::runtime_error("Invalid event size");
  }

  action_ = keyboard_event_data->action;
  const char* code_begin =
      static_cast<const char*>(data) + sizeof(KeyboardEventData);
  const char* code_end = code_begin + keyboard_event_data->code_length;
  const char* key_begin = code_end;
  const char* key_end = key_begin + keyboard_event_data->key_length;
  code_.append(code_begin, code_end);
  key_.append(key_begin, key_end);
}

std::vector<std::uint8_t> KeyboardEvent::Serialize() const {
  std::vector<std::uint8_t> buffer(sizeof(KeyboardEventData) + code_.length() +
                                   key_.length());

  assert(code_.length() < 256 && key_.length() < 256);

  auto keyboard_event_data =
      reinterpret_cast<KeyboardEventData*>(buffer.data());
  keyboard_event_data->event_type = EventType::KEYBOARD_INPUT;
  keyboard_event_data->action = action_;
  keyboard_event_data->code_length = static_cast<std::uint8_t>(code_.length());
  keyboard_event_data->key_length = static_cast<std::uint8_t>(key_.length());
  memcpy(buffer.data() + sizeof(KeyboardEventData), code_.c_str(),
         code_.length());
  memcpy(buffer.data() + sizeof(KeyboardEventData) + code_.length(),
         key_.c_str(), key_.length());

  return buffer;
}

std::string KeyboardEvent::ToString() const {
  // TODO(Simon): insert all properties
  return "KeyboardEvent(" + code_ + "," + key_ + ")";
}

}  // namespace webstreamer
