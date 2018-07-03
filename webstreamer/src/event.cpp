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

#include "webstreamer/event.hpp"
#include <cassert>
#include "webstreamer/codec_event.hpp"
#include "webstreamer/mouse_event.hpp"
#include "webstreamer/keyboard_event.hpp"

namespace webstreamer {

Event::Event(const void* data, std::size_t size_in_bytes) {
  (void)size_in_bytes;
  assert(size_in_bytes >= 1);
  static_assert(sizeof(EventType) == 1, "EventType has invalid size");
  type_ = *reinterpret_cast<const EventType*>(data);
}

std::vector<std::uint8_t> Event::Serialize() const {
  return {static_cast<std::uint8_t>(type())};
}

std::string Event::ToString() const {
  switch (type()) {
    case EventType::STREAM_CONFIG_CHANGED:
      return "STREAM_CONFIG_CHANGED";

    case EventType::PLAY:
      return "PLAY";

    case EventType::STOP:
      return "STOP";

    case EventType::AQUIRE_INPUT:
      return "AQUIRE_INPUT";

    case EventType::RELEASE_INPUT:
      return "RELEASE_INPUT";

    case EventType::CHANGE_CODEC:
      return "CHANGE_CODEC";

	case EventType::MOUSE_INPUT:
		return "MOUSE_INPUT";

    case EventType::UNKNOWN:
      return "UNKNOWN";

    default:
      return "Invalid(" + std::to_string(static_cast<int>(type())) + ")";
  }
}

Event::Ptr DeserializeEvent(const void* data, std::size_t size_in_bytes) {
  if (size_in_bytes == 0) {
    return {};
  }

  EventType type = *reinterpret_cast<const EventType*>(data);
  switch (type) {
    case EventType::STREAM_CONFIG_CHANGED:
    case EventType::PLAY:
    case EventType::STOP:
    case EventType::AQUIRE_INPUT:
    case EventType::RELEASE_INPUT:
      return std::make_unique<Event>(data, size_in_bytes);

	case EventType::CHANGE_CODEC:
		return std::make_unique<CodecEvent>(data, size_in_bytes);

	case EventType::MOUSE_INPUT:
		return std::make_unique<MouseEvent>(data, size_in_bytes);

	case EventType::KEYBOARD_INPUT:
		return std::make_unique<KeyboardEvent>(data, size_in_bytes);

    default:
      return std::make_unique<Event>(EventType::UNKNOWN);
  }
}

}  // namespace webstreamer
