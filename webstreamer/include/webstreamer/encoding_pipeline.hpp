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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_ENCODING_PIPELINE_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_ENCODING_PIPELINE_HPP_

#if !defined(_MSC_VER) || \
    _MSC_VER >= 1900  // shared_mutex is not supported on MSVC 12 and below
#define SHARED_MUTEX_SUPPORTED 1
#else
#define SHARED_MUTEX_SUPPORTED 0
#endif

#include <atomic>
#include <map>
#include <mutex>
#if !defined(_MSC_VER) || \
    _MSC_VER >= 1900  // shared_mutex is not supported on MSVC 12 and below
#include <shared_mutex>
#endif
#include <thread>
#include "webstreamer/encoder.hpp"
#include "webstreamer/export.hpp"
#include "webstreamer/frame_buffer.hpp"

namespace webstreamer {

class Client;
class EncoderFactory;

class WEBSTREAMER_EXPORT EncodingPipeline {
 public:
  EncodingPipeline();

  void RegisterEncoderFactoryForCodec(Codec codec, EncoderFactory* factory);

  bool RegisterClient(Client* client, Codec codec, const CodecOptions& options);
  void DeregisterClient(Client* client);

  // Each line in rgb_data must be aligned to 4 byte
  void PushFrame(std::size_t width, std::size_t height, const void* rgb_data,
                 std::size_t size_in_bytes, bool flip_vertically);

 private:
  std::map<Codec, EncoderFactory*> encoder_factories_;
  std::vector<std::unique_ptr<Encoder>> encoders_;
  std::vector<std::thread> encoder_threads_;
  std::mutex encoders_access_mutex_;

  FrameBuffer frame_buffers_[2];

  FrameBuffer* writing_frame_buffer_;
  std::atomic<std::size_t> writing_frame_index_;
  std::mutex writing_frame_buffer_mutex_;

  FrameBuffer* encoding_frame_buffer_;
  std::atomic<std::size_t> encoding_frame_index_;
#if SHARED_MUTEX_SUPPORTED
  std::shared_timed_mutex encoding_frame_buffer_mutex_;
#else
  std::mutex encoding_frame_buffer_mutex_;
#endif

  std::thread swap_thread_;

  void EncoderThread(Encoder* encoder);
  void SwapThread();
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_ENCODING_PIPELINE_HPP_
