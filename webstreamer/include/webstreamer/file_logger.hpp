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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_FILE_LOGGER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_FILE_LOGGER_HPP_

#include <fstream>
#include "webstreamer/export.hpp"
#include "webstreamer/log.hpp"

namespace webstreamer {

class WEBSTREAMER_EXPORT FileLogger : public LogListener {
 public:
  explicit FileLogger(LogLevel minimum_log_level = DEFAULT_LOG_LEVEL,
                      const std::string& filename = "");
  void Write(const tm& time, LogLevel level, const std::string& text) override;

 private:
  std::ofstream file_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_FILE_LOGGER_HPP_
