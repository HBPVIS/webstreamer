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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_RAW_ENCODER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_RAW_ENCODER_HPP_

#include "webstreamer/encoder.hpp"
#include "webstreamer/export.hpp"

namespace webstreamer {

class WEBSTREAMER_EXPORT RawEncoder : public Encoder {
 public:
  bool IsCompatible(const CodecOptions& configuration) override;

 protected:
  EncodedFrame EncodeFrame(const FrameBuffer& frame_buffer) override;

 private:
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_RAW_ENCODER_HPP_
