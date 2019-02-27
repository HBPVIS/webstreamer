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

#ifdef WEBSTREAMER_ENABLE_WEBRTC

#include "webstreamer/webrtc_stream_server.hpp"
#include "webrtc/pc/peerconnectionfactory.h"
#include "webstreamer/client_set.hpp"
#include "webstreamer/webrtc_stream_client.hpp"
#include "webstreamer/access_manager.hpp"

namespace webstreamer {

WebRTCStreamServer::WebRTCStreamServer(
    const Poco::Util::JSONConfiguration* webstreamer_configuration,
    Poco::Util::JSONConfiguration* stream_configuration, ClientSet* clients,
    int inputPort)
    : WebSocketServer(
          inputPort == -1 ? static_cast<std::uint16_t>(webstreamer_configuration->getUInt(
              "streams.webRTCStream.port", 8081)) : inputPort),
      stream_configuration_(stream_configuration),
      clients_(clients),
      signaling_thread_(&WebRTCStreamServer::SignalingThread, this) {
  stream_configuration_->setBool("streams.webRTC.supported", true);
  if (webstreamer_configuration->has("streams.webRTCStream.url")) {
    stream_configuration_->setString(
        "streams.webRTC.url",
        webstreamer_configuration->getString("streams.webRTCStream.url"));
  }
  stream_configuration_->setUInt("streams.webRTC.port", port());
}

void WebRTCStreamServer::OnConnect(const Poco::Net::WebSocket& web_socket) {
  // TODO(Simon): Make peer_connection_factory_ thread safe

  const std::string address = web_socket.peerAddress ( ).host ( ).toString ( );

  if ( AccessManager::getInstance ( ).addressIsAllowed ( address ))
  {
    clients_->Insert<WebRTCStreamClient>(
      std::move(web_socket), peer_connection_factory_.get(),
      webrtc::PeerConnectionInterface::RTCConfiguration{});
  }
}

void WebRTCStreamServer::SignalingThread() {
  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();
  rtc::Thread::Current()->Run();
}

}  // namespace webstreamer

#endif  // WEBSTREAMER_ENABLE_WEBRTC
