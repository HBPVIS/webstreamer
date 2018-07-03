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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_H264_ENCODER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_H264_ENCODER_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>
#pragma warning(push, 0)
#include "x264.h"
extern "C" {
#include "libswscale/swscale.h"
}
#pragma warning(pop)
#include "webstreamer/encoder.hpp"
#include "webstreamer/export.hpp"
#include "webstreamer/frame_buffer.hpp"
#include "webstreamer/resolution.hpp"
#include "webstreamer/stop_watch.hpp"

namespace webstreamer {

class WEBSTREAMER_EXPORT H264Encoder : public Encoder {
 public:
  H264Encoder(int width, int height, int framerate, int bitrate);
  ~H264Encoder() override;

  bool IsCompatible(const CodecOptions& configuration) override;

 protected:
  EncodedFrame EncodeFrame(const FrameBuffer& frame_buffer) override;

 private:
  void Reset();

  int input_width_;
  int input_height_;

  int output_width_;
  int output_height_;
  int framerate_;
  int bitrate_;

  bool needs_reset_;

  x264_param_t encoder_parameters_;
  x264_t* encoder_;
  x264_picture_t encoder_input_picture_;
  x264_picture_t encoder_output_picture_;
  SwsContext* sws_context_;

  std::vector<std::uint8_t> buffer_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_H264_ENCODER_HPP_
