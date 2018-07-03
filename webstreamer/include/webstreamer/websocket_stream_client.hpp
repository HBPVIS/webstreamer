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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_STREAM_CLIENT_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_STREAM_CLIENT_HPP_

#include <cstdint>
#include <mutex>
#include <thread>
#include "webstreamer/client.hpp"
#include "webstreamer/export.hpp"
#pragma warning(push, 0)
#include "Poco/Net/WebSocket.h"
#pragma warning(pop)

namespace webstreamer {

class WEBSTREAMER_EXPORT WebSocketStreamClient : public Client {
  enum class DataType : std::uint32_t {
    UNKNOWN = 0,
    EVENT_DATA = 1,
    ENCODED_FRAME = 2,
  };

 public:
  WebSocketStreamClient(Poco::Net::WebSocket web_socket);
  ~WebSocketStreamClient() override;

  void OnFrameEncoded(const EncodedFrame& encoded_frame) override;
  void OnCodecSwitched(Codec codec, const CodecOptions& options) override;
  void SendEvent(const Event& event) override;

 private:
  std::mutex buffer_mutex_;
  std::vector<std::uint8_t> buffer_;
  Poco::Net::WebSocket web_socket_;
  Poco::Net::SocketAddress address_;
  std::thread receive_thread_;

  void SendData(DataType data_type, const void* data, std::size_t data_size);
  void ReceiveThread();
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_STREAM_CLIENT_HPP_
