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

#include "webstreamer/raw_encoder.hpp"
#include "webstreamer/frame_buffer.hpp"

namespace webstreamer {

bool RawEncoder::IsCompatible(const CodecOptions& configuration) {
  (void)configuration;
  return true;
}

EncodedFrame RawEncoder::EncodeFrame(const FrameBuffer& frame_buffer) {
  EncodedFrame encoded_frame;
  encoded_frame.width = frame_buffer.width();
  encoded_frame.height = frame_buffer.height();
  encoded_frame.size_in_bytes = frame_buffer.stride() * frame_buffer.height();
  encoded_frame.data =
      reinterpret_cast<const std::uint8_t*>(frame_buffer.pixel_data());
  return encoded_frame;
}

}  // namespace webstreamer
