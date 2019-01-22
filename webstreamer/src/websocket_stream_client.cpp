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

#include "webstreamer/websocket_stream_client.hpp"
#include "log.hpp"
#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/Net/NetException.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

class WebSocketServer;

WebSocketStreamClient::WebSocketStreamClient(Poco::Net::WebSocket web_socket)
    : buffer_(4, 0),
      web_socket_(std::move(web_socket)),
      address_(web_socket_.peerAddress()),
      receive_thread_(&WebSocketStreamClient::ReceiveThread, this) {
  LOGI("WebSocketStreamClient connected: ", address_);
}

WebSocketStreamClient::~WebSocketStreamClient() {
  Die();
  receive_thread_.join();
  LOGI("WebSocketStreamClient disconnected: ", address_);
}

void WebSocketStreamClient::OnFrameEncoded(const EncodedFrame& encoded_frame) {
  SendData(DataType::ENCODED_FRAME, encoded_frame.data,
           encoded_frame.size_in_bytes);
}

void WebSocketStreamClient::OnCodecSwitched(Codec codec,
                                            const CodecOptions& options) {
  (void)codec;
  (void)options;
}

void WebSocketStreamClient::SendEvent(const Event& event) {
  const auto buffer = event.Serialize();
  SendData(DataType::ENCODED_FRAME, buffer.data(), buffer.size());
}

void WebSocketStreamClient::SendData(DataType data_type, const void* data,
                                     std::size_t data_size) {
  std::lock_guard<std::mutex> lock(buffer_mutex_);
  buffer_.resize(data_size + 4);
  std::memcpy(buffer_.data(), &data_type, 4);
  std::memcpy(buffer_.data() + 4, data, data_size);
  try {
    const int buffer_size = static_cast<int>(buffer_.size());
    if (web_socket_.sendBytes(buffer_.data(), buffer_size) != buffer_size) {
      LOGE("Client ", address_, " failed to send bytes");
      Die();
    }
  } catch (const Poco::Net::NetException& exception) {
    LOGE("Client ", address_, " failed to send bytes: ", exception.message());
    Die();
  }
}

void WebSocketStreamClient::ReceiveThread() {
  Poco::Buffer<char> buffer(0);
  int flags;

  while (is_alive()) {
    try {
      buffer.resize(0);
      int bytes_received = web_socket_.receiveFrame(buffer, flags);
      if (bytes_received == 0) {
        Die();
      } else {
        if (flags & Poco::Net::WebSocket::FRAME_OP_CLOSE) {
          Die();
        } else {
          LOGD("Received ", bytes_received, " byte(s) from ", address_);
          AddEvent(DeserializeEvent(buffer.begin(), bytes_received));
        }
      }
    } catch (const Poco::Net::NetException& exception) {
      LOGE("Client ", address_,
           " failed to receive bytes: ", exception.message());
      Die();
    } catch (const Poco::TimeoutException&) {
    }
  }
}

}  // namespace webstreamer
