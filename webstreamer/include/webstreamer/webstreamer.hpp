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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSTREAMER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSTREAMER_HPP_

#include <string>
#include <webstreamer/client_set.hpp>
#include <webstreamer/encoding_pipeline.hpp>
#include <webstreamer/input_processor.hpp>
#include <webstreamer/resolution.hpp>
#pragma warning(push, 0)
#include "Poco/Util/JSONConfiguration.h"
#pragma warning(pop)
#include "webstreamer/h264_encoder_factory.hpp"
#include "webstreamer/raw_encoder_factory.hpp"
#include "webstreamer/web_server.hpp"
#include "webstreamer/webrtc_stream_server.hpp"
#include "webstreamer/websocket_stream_server.hpp"

namespace webstreamer {

enum class StreamingProtocol {
  FILE_DUMP,
  WEBRTC,
  WEBSOCKET,
};

const std::vector<Resolution> DEFAULT_RESOLUTIONS = {{426, 240, 1050, 60},
                                                     {640, 360, 1500, 60},
                                                     {854, 480, 3000, 60},
                                                     {1280, 720, 6000, 60},
                                                     {1920, 1080, 9000, 60}};

class WEBSTREAMER_EXPORT WebStreamer final {
 public:
  WebStreamer(
      const std::string& configuration_path = "webstreamer_config.json");

  InputProcessor* RegisterInputProcessor(InputProcessor* input_processor);
  InputProcessor* DeregisterInputProcessor();
  inline InputProcessor* current_input_processor() {
    return current_input_processor_;
  }

  // Each line in rgb_data must be aligned to 4 byte
  void PushFrame(std::size_t width, std::size_t height, const void* rgb_data,
                 std::size_t size_in_bytes, bool flip_vertically = false);

 private:
  Poco::Util::JSONConfiguration configuration_;
  Poco::Util::JSONConfiguration stream_config_;
  WebServer web_server_;
  InputProcessor* current_input_processor_;
  EncodingPipeline encoding_pipeline_;
  ClientSet clients_;

  // Streams
  WebSocketStreamServer websocket_stream_;
#ifdef WEBSTREAMER_ENABLE_WEBRTC
  WebRTCStreamServer webrtc_stream_;
#endif

  // Codecs
  RawEncoderFactory raw_encoder_factory_;
  H264EncoderFactory h264_encoder_factory_;
};

}  // namespace webstreamer

#endif  // STREAMING_INCLUDE_STREAMING_ENCODER_HPP_
