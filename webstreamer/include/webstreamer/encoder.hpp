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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_ENCODER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_ENCODER_HPP_

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>
#include "webstreamer/export.hpp"
#include "webstreamer/stop_watch.hpp"
#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/Util/JSONConfiguration.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

class Client;
class FrameBuffer;

enum class Codec {
  RAW,
  H264,
};

typedef Poco::JSON::Object CodecOptions;

struct WEBSTREAMER_EXPORT EncodedFrame {
  std::size_t width;
  std::size_t height;
  std::size_t size_in_bytes;
  const std::uint8_t* data;
};

class WEBSTREAMER_EXPORT Encoder {
 public:
  Encoder();
  virtual ~Encoder() = default;

  inline Codec codec() const { return codec_; }
  inline const CodecOptions& codec_options() const { return codec_options_; }

  virtual bool IsCompatible(const CodecOptions& configuration) = 0;

  void RegisterClient(Client* client);
  void DeregisterClient(Client* client);

  void PushFrame(const FrameBuffer& frame_buffers);

  inline StopWatch<>::Duration idle_time() { return idle_time_.elapsed_time(); }

 protected:
  inline bool has_new_client() const { return has_new_client_; }
  virtual EncodedFrame EncodeFrame(const FrameBuffer& frame_buffer) = 0;

 private:
  CodecOptions codec_options_;
  Codec codec_;

  std::vector<Client*> clients_;
  std::mutex clients_access_mutex_;
  bool has_new_client_ = false;

  StopWatch<> idle_time_;

  void SendEncodedFrameToRegisteredClients(const EncodedFrame& encoded_frame);
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_ENCODER_HPP_
