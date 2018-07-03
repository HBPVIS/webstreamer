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

#include "webstreamer/input_processor.hpp"
#include "log.hpp"
#include "webstreamer/down_cast.hpp"
#include "webstreamer/keyboard_event.hpp"
#include "webstreamer/mouse_event.hpp"

namespace webstreamer {

void InputProcessor::ProcessMouseInput(const MouseEvent&) {
  LogD("Received mouse event!");
}

void InputProcessor::ProcessKeyboardInput(const KeyboardEvent&) {
  LogD("Received keyboard event!");
}

void InputProcessor::ProcessEvent(const Event& event) {
  switch (event.type()) {
    case EventType::MOUSE_INPUT:
      ProcessMouseInput(down_cast<const MouseEvent&>(event));
      break;

    case EventType::KEYBOARD_INPUT:
      ProcessKeyboardInput(down_cast<const KeyboardEvent&>(event));
      break;

    default:
      LOGD("Invalid event passed to ProcessEvent() (must be an input event)");
  }
}

void SynchronousInputProcessor::PushEvent(Event::Ptr event) {
  std::lock_guard<std::mutex> lock(events_mutex_);
  events_.emplace_back(std::move(event));
}

void SynchronousInputProcessor::ProcessInput() {
  std::lock_guard<std::mutex> lock(events_mutex_);
  for (const auto& event : events_) {
    ProcessEvent(*event);
  }
  events_.clear();
}

void AsynchronousInputProcessor::PushEvent(Event::Ptr event) {
  ProcessEvent(*event);
}

}  // namespace webstreamer
