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

#include <cassert>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include <webstreamer/event.hpp>
#include <webstreamer/h264_encoder.hpp>
#include <webstreamer/std_function_delegate.hpp>
#include <webstreamer/webstreamer.hpp>
#include "log.hpp"

#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/JSON/Parser.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

WebStreamer::WebStreamer(int webPort,
                         int webSocketPort,
                         int webRtcPort,
                         const std::string& configuration_path)
    : configuration_(configuration_path),
      web_server_(&stream_config_,
                  configuration_.getString("webServer.rootDir", ""),
                  webPort == -1? static_cast<std::uint16_t>(
                      configuration_.getInt("webServer.port", 80)) : webPort),
      current_input_processor_(nullptr),
      clients_(&encoding_pipeline_),
      websocket_stream_(&configuration_, &stream_config_, &clients_, webSocketPort),
#ifdef WEBSTREAMER_ENABLE_WEBRTC
      webrtc_stream_(&configuration_, &stream_config_, &clients_, webRtcPort),
#endif
      raw_encoder_factory_(&configuration_, &stream_config_),
      h264_encoder_factory_(&configuration_, &stream_config_) {

#ifndef WEBSTREAMER_ENABLE_WEBRTC
  std::cout << "No webrtc support. Designed port was " << webRtcPort << std::endl;
#endif
  stream_config_.propertyChanged +=
      StdFunctionDelegate<const Poco::Util::AbstractConfiguration::KeyValue>(
          [this](const void*,
                 const Poco::Util::AbstractConfiguration::KeyValue&) {
            clients_.OnStreamConfigChanged();
          });

  stream_config_.propertyRemoved += StdFunctionDelegate<const std::string>(
      [this](const void*, const std::string&) {
        clients_.OnStreamConfigChanged();
      });

  encoding_pipeline_.RegisterEncoderFactoryForCodec(Codec::RAW,
                                                    &raw_encoder_factory_);

  encoding_pipeline_.RegisterEncoderFactoryForCodec(Codec::H264,
                                                    &h264_encoder_factory_);
}

InputProcessor* WebStreamer::RegisterInputProcessor(
    InputProcessor* input_processor) {
  if (current_input_processor_ == input_processor) {
    return current_input_processor_;
  }

  auto previous_input_processor = current_input_processor_;
  clients_.SetInputProcessor(input_processor);
  current_input_processor_ = input_processor;
  return previous_input_processor;
}

InputProcessor* WebStreamer::DeregisterInputProcessor() {
  return RegisterInputProcessor(nullptr);
}

void WebStreamer::PushFrame(std::size_t width, std::size_t height,
                            const void* rgb_data, std::size_t size_in_bytes,
                            bool flip_vertically) {
  encoding_pipeline_.PushFrame(width, height, rgb_data, size_in_bytes,
                               flip_vertically);
}

}  // namespace webstreamer
