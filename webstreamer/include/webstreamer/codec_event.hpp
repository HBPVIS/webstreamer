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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_CODEC_EVENT_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_CODEC_EVENT_HPP_

#include <cstddef>
#include "webstreamer/encoder.hpp"
#include "webstreamer/event.hpp"

namespace webstreamer {

class WEBSTREAMER_EXPORT CodecEvent : public Event {
 public:
  CodecEvent(const void* data, std::size_t size_in_bytes);

  std::vector<std::uint8_t> Serialize() const override;
  std::string ToString() const override;

  inline Codec codec() const { return codec_; }
  inline const CodecOptions& options() const { return options_; }

 private:
  CodecOptions options_;
  Codec codec_;

  std::string CreateOptionsString() const;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_CODEC_EVENT_HPP_
