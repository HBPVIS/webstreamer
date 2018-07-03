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

#include "webstreamer/encoding_pipeline.hpp"
#include <cassert>
#include <cstdint>
#include "log.hpp"
#include "webstreamer/encoder.hpp"
#include "webstreamer/encoder_factory.hpp"
#include "webstreamer/raw_encoder.hpp"

namespace webstreamer {

EncodingPipeline::EncodingPipeline()
    : writing_frame_buffer_(&frame_buffers_[0]),
      encoding_frame_buffer_(&frame_buffers_[1]),
      encoding_frame_index_(0),
      writing_frame_index_(0),
      swap_thread_(&EncodingPipeline::SwapThread, this) {}

void EncodingPipeline::RegisterEncoderFactoryForCodec(Codec codec,
                                                      EncoderFactory* factory) {
  assert(encoder_factories_.count(codec) == 0);
  assert(factory != 0);
  encoder_factories_[codec] = factory;
}

bool EncodingPipeline::RegisterClient(Client* client, Codec codec,
                                      const CodecOptions& options) {
  std::lock_guard<std::mutex> lock(encoders_access_mutex_);
  for (const auto& encoder : encoders_) {
    if (encoder->IsCompatible(options)) {
      encoder->RegisterClient(client);
      return true;
    }
  }

  if (encoder_factories_.count(codec) != 1) {
    return false;
  }

  try {
    std::unique_ptr<Encoder> encoder =
        encoder_factories_[codec]->CreateEncoder(options);

    encoder->RegisterClient(client);
    encoders_.push_back(std::move(encoder));
    encoder_threads_.emplace_back(&EncodingPipeline::EncoderThread, this,
                                  encoders_.back().get());
  } catch (const Poco::Exception&) {
    return false;
  }

  return true;
}

void EncodingPipeline::DeregisterClient(Client* client) {
  for (auto& encoder : encoders_) {
    encoder->DeregisterClient(client);
  }
}

void EncodingPipeline::PushFrame(std::size_t width, std::size_t height,
                                 const void* rgb_data,
                                 std::size_t size_in_bytes,
                                 bool flip_vertically) {
  (void)size_in_bytes;
  std::lock_guard<std::mutex> writing_lock(writing_frame_buffer_mutex_);

  writing_frame_buffer_->ResizeIfNecessary(width, height);
  assert(size_in_bytes == writing_frame_buffer_->size_in_bytes());
  if (flip_vertically) {
    auto current_source_line = reinterpret_cast<const std::uint8_t*>(rgb_data);
    const std::size_t stride =
        writing_frame_buffer_->stride();  // The alignment of the source data
                                          // has the same requirements as the
                                          // alignment in the frame buffer

    for (std::size_t row = height; row != 0;
         --row, current_source_line += stride) {
      std::memcpy(writing_frame_buffer_->GetRowData(row - 1),
                  current_source_line, stride);
    }
  } else {
    std::memcpy(writing_frame_buffer_->pixel_data(), rgb_data,
                writing_frame_buffer_->size_in_bytes());
  }

  // Relaxed is fine here, as this is the only thread that writes to that value
  const std::size_t writing_frame_index =
      writing_frame_index_.load(std::memory_order_relaxed);
  writing_frame_index_.store(writing_frame_index + 1,
                             std::memory_order_release);
}

void EncodingPipeline::EncoderThread(Encoder* encoder) {
  std::size_t last_encoded_frame_index_ = 0;
  while (true) {
    const std::size_t approximate_encoding_frame_index =
        encoding_frame_index_.load(std::memory_order_relaxed);

    const auto now = std::chrono::high_resolution_clock::now();

    if (approximate_encoding_frame_index != last_encoded_frame_index_) {
#if SHARED_MUTEX_SUPPORTED
      std::shared_lock<std::shared_timed_mutex> encoding_lock(
          encoding_frame_buffer_mutex_);
#else
      std::lock_guard<std::mutex> encoding_lock(encoding_frame_buffer_mutex_);
#endif

      LOGV("Encode frame from: ", encoding_frame_buffer_);
      encoder->PushFrame(*encoding_frame_buffer_);
      const std::size_t actual_encoding_frame_index =
          encoding_frame_index_.load(std::memory_order_acquire);

      last_encoded_frame_index_ = actual_encoding_frame_index;
    } else {
      std::this_thread::yield();
    }
  }
}

void EncodingPipeline::SwapThread() {
  while (true) {
    const std::size_t approximate_writing_frame_index =
        writing_frame_index_.load(std::memory_order_relaxed);

    if (approximate_writing_frame_index != encoding_frame_index_) {
#if SHARED_MUTEX_SUPPORTED
      std::shared_lock<std::shared_timed_mutex> encoding_lock(
          encoding_frame_buffer_mutex_);
#else
      std::lock_guard<std::mutex> encoding_lock(encoding_frame_buffer_mutex_);
#endif
      std::lock_guard<std::mutex> writing_lock(writing_frame_buffer_mutex_);

      std::swap(encoding_frame_buffer_, writing_frame_buffer_);

      const std::size_t actual_writing_frame_index =
          writing_frame_index_.load(std::memory_order_acquire);
      encoding_frame_index_.store(actual_writing_frame_index,
                                  std::memory_order_release);
    } else {
      std::this_thread::yield();
    }
  }
}

}  // namespace webstreamer
