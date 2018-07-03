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

#ifndef WEBSTREAMER_SRC_LOG_HPP_
#define WEBSTREAMER_SRC_LOG_HPP_

#include <cstring>
#include "webstreamer/log.hpp"

namespace webstreamer {
namespace detail {

inline const char* ExtractFilename(const char* filename) {
  const char* result = filename;

  while (*filename != '\0') {
    if (*filename == '/' || *filename == '\\') {
      result = filename + 1;
    }
    ++filename;
  }

  return result;
}

}  // namespace detail
}  // namespace webstreamer

#define WEBSTREAMER_LOG(level, ...)                                         \
  ::webstreamer::Log(level, __VA_ARGS__, " (",                              \
                     ::webstreamer::detail::ExtractFilename(__FILE__), ":", \
                     __LINE__, ")")
#define LOGV(...) \
  WEBSTREAMER_LOG(::webstreamer::LogLevel::VERBOSE, __VA_ARGS__);
#define LOGD(...) WEBSTREAMER_LOG(::webstreamer::LogLevel::DEBUG, __VA_ARGS__);
#define LOGI(...) \
  WEBSTREAMER_LOG(::webstreamer::LogLevel::INFORMATION, __VA_ARGS__);
#define LOGW(...) \
  WEBSTREAMER_LOG(::webstreamer::LogLevel::WARNING, __VA_ARGS__);
#define LOGE(...) WEBSTREAMER_LOG(::webstreamer::LogLevel::ERROR, __VA_ARGS__);
#define LOGF(...) WEBSTREAMER_LOG(::webstreamer::LogLevel::FATAL, __VA_ARGS__);

#endif  // WEBSTREAMER_SRC_LOG_HPP_
