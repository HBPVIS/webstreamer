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

#include "webstreamer/mouse_event.hpp"
#include <cassert>
#include <stdexcept>

namespace webstreamer {

namespace {

struct MouseEventData {
  EventType event_type;
  MouseAction action;
  std::uint16_t x;
  std::uint16_t y;
  MouseButton button;
  MouseButtons buttons;
};
static_assert(sizeof(MouseEventData) == 8, "Invalid padding");

}  // namespace

MouseEvent::MouseEvent(const void* data, std::size_t size_in_bytes)
    : Event(EventType::MOUSE_INPUT) {
  if (size_in_bytes != sizeof(MouseEventData)) {
    throw std::runtime_error("Invalid event size");
  }

  auto mouse_event_data = reinterpret_cast<const MouseEventData*>(data);
  assert(mouse_event_data->event_type == EventType::MOUSE_INPUT);

  action_ = mouse_event_data->action;
  x_ = static_cast<double>(mouse_event_data->x) / static_cast<double>(0xffff);
  y_ = static_cast<double>(mouse_event_data->y) / static_cast<double>(0xffff);
  button_ = mouse_event_data->button;
  buttons_ = mouse_event_data->buttons;
}

std::vector<std::uint8_t> MouseEvent::Serialize() const {
  std::vector<std::uint8_t> buffer(sizeof(MouseEventData));

  auto mouse_event_data = reinterpret_cast<MouseEventData*>(buffer.data());
  mouse_event_data->event_type = EventType::MOUSE_INPUT;
  mouse_event_data->action = action_;
  mouse_event_data->x =
      static_cast<std::uint16_t>(x_ * static_cast<double>(0xffff));
  mouse_event_data->y =
      static_cast<std::uint16_t>(y_ * static_cast<double>(0xffff));
  mouse_event_data->button = button_;
  mouse_event_data->buttons = buttons_;

  return buffer;
}

std::string MouseEvent::ToString() const {
  // TODO(Simon): insert all properties
  return "MouseEvent";
}

}  // namespace webstreamer