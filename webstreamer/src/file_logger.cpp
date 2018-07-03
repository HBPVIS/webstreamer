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

#include "webstreamer/file_logger.hpp"
#include <iomanip>

namespace webstreamer {

FileLogger::FileLogger(LogLevel minimum_log_level, const std::string& filename)
    : LogListener(minimum_log_level) {
  if (filename == "") {
    std::time_t t = std::time(nullptr);

    std::stringstream buffer;
    buffer << std::put_time(std::localtime(&t), "%Y-%m-%d %H-%M-%S.log");
    file_.open(buffer.str());
  } else {
    file_.open(filename);
  }
  if (!file_.is_open()) {
    throw new std::runtime_error("Failed to create FileLogger");
  }
}

void FileLogger::Write(const tm& time, LogLevel level,
                       const std::string& text) {
  file_ << std::put_time(&time, "%Y-%m-%d %H:%M:%S") << " [" << GetLogLevelCharacter(level)
        << "] " << text << "\n";
}

}  // namespace webstreamer
