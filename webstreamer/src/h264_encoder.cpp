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

#include "webstreamer/h264_encoder.hpp"
#include <iostream>
#include "log.hpp"
#include "webstreamer/stop_watch.hpp"

namespace webstreamer {

namespace {

struct EncodedFrameInfo {
  std::uint32_t frame_index;
  std::uint16_t resolution_width;
  std::uint16_t resolution_height;
};
static_assert(sizeof(EncodedFrameInfo) == 8, "");

}  // namespace

H264Encoder::H264Encoder(int width, int height, int framerate, int bitrate)
    : input_width_(0),
      input_height_(0),
      output_width_(width),
      output_height_(height),
      framerate_(framerate),
      bitrate_(bitrate),
      needs_reset_(true),
      encoder_(nullptr),
      sws_context_(nullptr) {}

H264Encoder::~H264Encoder() {
  if (encoder_ != nullptr) {
    x264_encoder_close(encoder_);
    x264_picture_clean(&encoder_input_picture_);
  }

  sws_freeContext(sws_context_);
}

bool H264Encoder::IsCompatible(const CodecOptions& options) {
  return options.optValue<int>("width", output_width_) == output_width_ &&
         options.optValue<int>("height", output_height_) == output_height_ &&
         options.optValue<int>("framerate", framerate_) == framerate_ &&
         options.optValue<int>("bitrate", bitrate_) == bitrate_;
}

void H264Encoder::Reset() {
  x264_param_default_preset(&encoder_parameters_, "ultrafast", "zerolatency");
  encoder_parameters_.i_width = output_width_;
  encoder_parameters_.i_height = output_height_;
  encoder_parameters_.rc.i_rc_method = X264_RC_ABR;
  encoder_parameters_.rc.i_bitrate = bitrate_;
  encoder_parameters_.i_fps_num = framerate_;
  encoder_parameters_.i_fps_den = 1;
  encoder_parameters_.b_annexb = 1;
  encoder_parameters_.analyse.i_weighted_pred = X264_WEIGHTP_NONE;
  x264_param_apply_fastfirstpass(&encoder_parameters_);
  x264_param_apply_profile(&encoder_parameters_, "baseline");

  if (encoder_ != nullptr) {
    if (x264_encoder_reconfig(encoder_, &encoder_parameters_) != 0) {
      LOGE("Failed to configure x264 encoder");
    }

    x264_picture_clean(&encoder_input_picture_);
  } else {
    encoder_ = x264_encoder_open(&encoder_parameters_);

    if (encoder_ == nullptr) {
      LOGE("Failed to create x264 encoder");
    }
  }

  if (x264_picture_alloc(&encoder_input_picture_, X264_CSP_I420, output_width_,
                         output_height_) != 0) {
    LOGE("Failed to allocate picture");
  }

  if (!sws_isSupportedInput(AV_PIX_FMT_RGB24)) {
    LOGE("Invalid input format: RGB24");
  }

  if (!sws_isSupportedOutput(AV_PIX_FMT_YUV420P)) {
    LOGE("Invalid output format: YUV420P");
  }

  sws_context_ =
      sws_getCachedContext(sws_context_, input_width_, input_height_,
                           AV_PIX_FMT_RGB24, output_width_, output_height_,
                           AV_PIX_FMT_YUV420P, 0, nullptr, nullptr, nullptr);

  if (!sws_context_) {
    LOGE("Failed to initialize sws context");
  }

  needs_reset_ = false;
}

EncodedFrame H264Encoder::EncodeFrame(const FrameBuffer& frame_buffer) {
  EncodedFrame encoded_frame;
  encoded_frame.width = 0;
  encoded_frame.height = 0;
  encoded_frame.size_in_bytes = 0;
  encoded_frame.data = nullptr;

  if (frame_buffer.width() == 0 || frame_buffer.height() == 0) {
    LOGW("Invalid frame dimensions: ", frame_buffer.width(), "x",
         frame_buffer.height());
    return encoded_frame;
  }

  if (frame_buffer.width() != static_cast<std::size_t>(input_width_) ||
      frame_buffer.height() != static_cast<std::size_t>(input_height_)) {
    input_width_ = static_cast<int>(frame_buffer.width());
    input_height_ = static_cast<int>(frame_buffer.height());
    needs_reset_ = true;
  }
  if (needs_reset_) {
    Reset();
  }
  const std::uint8_t* src_slice[] = {
      reinterpret_cast<const std::uint8_t*>(frame_buffer.pixel_data()),
      nullptr};
  const int src_stride[] = {static_cast<int>(frame_buffer.stride()), 0};
  const int src_slice_y = 0;
  const int src_slice_h = static_cast<int>(frame_buffer.height());

  const int dst_height = sws_scale(
      sws_context_, src_slice, src_stride, src_slice_y, src_slice_h,
      encoder_input_picture_.img.plane, encoder_input_picture_.img.i_stride);
  encoder_input_picture_.i_type =
      has_new_client() ? X264_TYPE_KEYFRAME : X264_TYPE_AUTO;

  if (dst_height != output_height_) {
    LOGW("Invalid height");
  }

  x264_nal_t* nals;
  int nal_count;
  if (x264_encoder_encode(encoder_, &nals, &nal_count, &encoder_input_picture_,
                          &encoder_output_picture_) < 0) {
    LOGW("Failed to encode frame");
  } else {
    buffer_.clear();
    for (int i = 0; i < nal_count; ++i) {
      buffer_.insert(buffer_.end(), nals[i].p_payload,
                     nals[i].p_payload + nals[i].i_payload);
    }
    encoded_frame.width = output_width_;
    encoded_frame.height = output_height_;
    encoded_frame.data = buffer_.data();
    encoded_frame.size_in_bytes = buffer_.size();
  }

  return encoded_frame;
}

}  // namespace webstreamer
