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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_STOP_WATCH_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_STOP_WATCH_HPP_

#include <cassert>
#include <chrono>
#include "webstreamer/export.hpp"

namespace webstreamer {

template <typename Clock = std::chrono::steady_clock>
class WEBSTREAMER_EXPORT StopWatch {
 public:
  typedef typename Clock::duration Duration;
  typedef typename Clock::time_point TimePoint;

  StopWatch() : is_running_(false), elapsed_time_(0) {}

  explicit StopWatch(bool start) : StopWatch() {
    if (start) {
      Start();
    }
  }

  inline void Start() {
    if (!is_running_) {
      last_time_ = Clock::now();
      is_running_ = true;
    }
  }

  inline void Stop() {
    if (is_running_) {
      UpdateElapsedTime();
      is_running_ = false;
    }
  }

  template <typename ReturnType = Duration>
  inline ReturnType Reset() {
    return Set(Duration(0));
  }

  template <typename ReturnType = Duration>
  inline ReturnType Set(Duration time) {
    if (is_running_) {
      UpdateElapsedTime();
      last_time_ = Clock::now();
    }
    const auto elapsed_time = elapsed_time_;
    elapsed_time_ = time;
    return std::chrono::duration_cast<ReturnType>(elapsed_time);
  }

  inline void Add(Duration time_to_add) { elapsed_time_ += time_to_add; }

  inline void Subtract(Duration time_to_subtract) {
    if (is_running_) {
      UpdateElapsedTime();
    }
    assert(elapsed_time_ >= time_to_subtract);
    elapsed_time_ -= time_to_subtract;
  }

  template <typename D = Duration>
  inline D elapsed_time() {
    if (is_running_) {
      UpdateElapsedTime();
    }
    return std::chrono::duration_cast<D>(elapsed_time_);
  }

  inline bool is_running() const { return is_running_; }

 private:
  bool is_running_;
  TimePoint last_time_;
  Duration elapsed_time_;

  inline void UpdateElapsedTime() {
    TimePoint now = Clock::now();
    elapsed_time_ += now - last_time_;
    last_time_ = now;
  }
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_STOP_WATCH_HPP_
