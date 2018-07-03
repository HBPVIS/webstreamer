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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_FRAME_BUFFER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_FRAME_BUFFER_HPP_

#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include "webstreamer/export.hpp"

namespace webstreamer {

struct FrameInfo {
  std::chrono::high_resolution_clock::time_point push_timestamp;
};

class WEBSTREAMER_EXPORT FrameBuffer {
 public:
  inline FrameBuffer() : FrameBuffer(0, 0) {}
  inline FrameBuffer(std::size_t width, std::size_t height)
      : width_(0), height_(0) {
    ResizeIfNecessary(width, height);
  }

  inline std::size_t width() const { return width_; }
  inline std::size_t height() const { return height_; }
  inline std::size_t stride() const { return stride_; }
  inline std::size_t bytes_per_pixel() const { return 3; }
  inline std::size_t size_in_bytes() const { return pixels_.size(); }

  inline void* pixel_data() { return pixels_.data(); }
  inline const void* pixel_data() const { return pixels_.data(); }

  inline void* GetRowData(std::size_t row) {
    assert(row < height_);
    return pixels_.data() + (row * stride());
  }
  inline const void* GetRowData(std::size_t row) const {
    assert(row < height_);
    return pixels_.data() + (row * stride());
  }

  inline FrameInfo& info() { return info_; }
  inline const FrameInfo& info() const { return info_; }

  inline void ResizeIfNecessary(std::size_t new_width, std::size_t new_height) {
    if (new_width != width_ || new_height != height_) {
      width_ = new_width;
      height_ = new_height;

      // Calculate stride
      const std::size_t bytes_per_line = bytes_per_pixel() * new_width;
      stride_ = ((bytes_per_line + 3) / 4) * 4;
      pixels_.resize(stride_ * height_);
    }
  }

 private:
  std::size_t width_;
  std::size_t stride_;
  std::size_t height_;
  std::vector<std::uint8_t> pixels_;
  FrameInfo info_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_FRAME_BUFFER_HPP_
