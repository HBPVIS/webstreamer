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

#include <webstreamer/log.hpp>

#include <cassert>
#include <vector>
#include <algorithm>

namespace webstreamer {

namespace {

static std::vector<LogListener*> g_log_listeners;
static std::vector<std::unique_ptr<LogListener>> g_immortal_log_listeners;

}  // namespace

void Log(const tm& time, LogLevel level, const std::string& text) {
  for (const auto& listener : g_log_listeners) {
    if (level >= listener->minimum_log_level()) {
      listener->Write(time, level, text);
    }
  }
}

LogListener::LogListener(LogLevel minimum_log_level)
    : minimum_log_level_(minimum_log_level) {
  g_log_listeners.push_back(this);
}

LogListener::~LogListener() {
  const auto this_iterator = std::find(g_log_listeners.begin(),
                                       g_log_listeners.end(), this);
  assert(this_iterator != g_log_listeners.end());
  std::swap(*this_iterator, g_log_listeners.back());
  g_log_listeners.pop_back();
}

namespace detail {

void MakeLogListenerImmortal(std::unique_ptr<LogListener>&& listener) {
  g_immortal_log_listeners.push_back(std::move(listener));
}

}

}  // namespace webstreamer
