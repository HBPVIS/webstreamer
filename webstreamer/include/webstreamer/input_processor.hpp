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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_INPUT_PROCESSOR_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_INPUT_PROCESSOR_HPP_

#include <mutex>
#include <vector>
#include "webstreamer/event.hpp"
#include "webstreamer/export.hpp"

namespace webstreamer {

class MouseEvent;
class KeyboardEvent;

// This class implements the input processing functionality. Do not derive from
// this class directly, but from SynchronousInputProcessor or
// AsynchronousInputProcessor.
class WEBSTREAMER_EXPORT InputProcessor {
 public:
  virtual ~InputProcessor() = default;

  virtual void PushEvent(Event::Ptr event) = 0;

  virtual void ProcessMouseInput(const MouseEvent& event);
  virtual void ProcessKeyboardInput(const KeyboardEvent& event);

 protected:
  void ProcessEvent(const Event& event);
};

// Derive from this class if you want synchronous event processing. E.g. you
// have to call ProcessInput() yourself in the application.
class WEBSTREAMER_EXPORT SynchronousInputProcessor : public InputProcessor {
 public:
  void PushEvent(Event::Ptr event) final;

  void ProcessInput();

 protected:
  std::mutex events_mutex_;
  std::vector<Event::Ptr> events_;
};

// Derive from this class if you want asynchronous input processing. This means
// ProcessMouseInput() and ProcessKeyboardInput() are called as soon the event
// arrives. However, the AsynchronousInputProcessor will still only process
// multiple events after one another.
class WEBSTREAMER_EXPORT AsynchronousInputProcessor : public InputProcessor {
 public:
  void PushEvent(Event::Ptr event) final;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_INPUT_PROCESSOR_HPP_
