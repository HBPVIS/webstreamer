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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_LOG_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_LOG_HPP_

#include <cassert>
#include <ctime>
#include <memory>
#include <sstream>
#include <type_traits>
#include "webstreamer/export.hpp"

namespace webstreamer {

// Poco defines ERROR to something else, this is a temporary fix to avoid the
// resulting errors.
#ifdef ERROR
#undef ERROR
#endif
enum class LogLevel {
  VERBOSE,
  DEBUG,
  INFORMATION,
  WARNING,
  ERROR,
  FATAL,
};

#ifdef NDEBUG
static const LogLevel DEFAULT_LOG_LEVEL = LogLevel::INFORMATION;
#else
static const LogLevel DEFAULT_LOG_LEVEL = LogLevel::DEBUG;
#endif

class WEBSTREAMER_EXPORT LogListener {
 public:
  explicit LogListener(LogLevel minimum_log_level = DEFAULT_LOG_LEVEL);
  virtual ~LogListener();

  // Make the class non-copyable
  LogListener(const LogListener&) = delete;
  LogListener& operator=(const LogListener&) = delete;

  inline LogLevel minimum_log_level() const { return minimum_log_level_; }

  virtual void Write(const tm& time, LogLevel level,
                     const std::string& text) = 0;

 private:
  LogLevel minimum_log_level_;
};

void Log(const tm& time, LogLevel level, const std::string& text);

namespace detail {
template <typename T>
void InsertToStream(std::stringstream& stream, const T& value) {
  stream << value;
}

template <typename T, typename... Ts>
void InsertToStream(std::stringstream& stream, const T& value, Ts&&... values) {
  stream << value;
  InsertToStream(stream, std::forward<Ts>(values)...);
}

void MakeLogListenerImmortal(std::unique_ptr<LogListener>&& listener);
}  // namespace detail

template <typename... T>
void Log(LogLevel log_level, T&&... arguments) {
  const std::time_t time = std::time(nullptr);
  std::stringstream stream;
  detail::InsertToStream(stream, arguments...);
  Log(*std::localtime(&time), log_level, stream.str());
}

template <typename... T>
void LogV(T&&... arguments) {
  Log(LogLevel::VERBOSE, std::forward<T>(arguments)...);
}

template <typename... T>
void LogD(T&&... arguments) {
  Log(LogLevel::DEBUG, std::forward<T>(arguments)...);
}

template <typename... T>
void LogI(T&&... arguments) {
  Log(LogLevel::INFORMATION, std::forward<T>(arguments)...);
}

template <typename... T>
void LogW(T&&... arguments) {
  Log(LogLevel::WARNING, std::forward<T>(arguments)...);
}

template <typename... T>
void LogE(T&&... arguments) {
  Log(LogLevel::ERROR, std::forward<T>(arguments)...);
}

template <typename... T>
void LogF(T&&... arguments) {
  Log(LogLevel::FATAL, std::forward<T>(arguments)...);
}

inline char GetLogLevelCharacter(LogLevel log_level) {
  switch (log_level) {
    case LogLevel::VERBOSE:
      return 'V';
    case LogLevel::DEBUG:
      return 'D';
    case LogLevel::INFORMATION:
      return 'I';
    case LogLevel::WARNING:
      return 'W';
    case LogLevel::ERROR:
      return 'E';
    case LogLevel::FATAL:
      return 'F';
  }
  assert(false);
  return ' ';  //  Keep the compiler from complaining
}

inline const char* GetLogLevelString(LogLevel log_level) {
  switch (log_level) {
    case LogLevel::VERBOSE:
      return "verbose";
    case LogLevel::DEBUG:
      return "debug";
    case LogLevel::INFORMATION:
      return "info";
    case LogLevel::WARNING:
      return "warning";
    case LogLevel::ERROR:
      return "error";
    case LogLevel::FATAL:
      return "fatal";
  }
  assert(false);
  return "";  //  Keep the compiler from complaining
}

template <typename Listener, typename... Arguments>
void CreateImmortalLogListener(Arguments&&... arguments) {
  static_assert(
      std::is_constructible<Listener, Arguments...>::value,
      "The log listener cannot be constructed using the provided arguments");
  static_assert(std::is_base_of<LogListener, Listener>::value,
                "The log listener must derive from webstreamer::LogListener");
  detail::MakeLogListenerImmortal(
      std::make_unique<Listener>(std::forward<Arguments>(arguments)...));
}

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_LOG_HPP_
