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

#include "webstreamer/websocket_stream_server.hpp"
#include "log.hpp"
#include "webstreamer/client_set.hpp"
#include "webstreamer/websocket_stream_client.hpp"
#include "webstreamer/access_manager.hpp"

namespace webstreamer {

WebSocketStreamServer::WebSocketStreamServer(
    const Poco::Util::JSONConfiguration* webstreamer_configuration,
    Poco::Util::JSONConfiguration* stream_configuration, ClientSet* clients,
    int inputPort)
    : WebSocketServer(
          inputPort == -1 ? static_cast<std::uint16_t>(webstreamer_configuration->getUInt(
              "streams.webSocketStream.port", 8080)) : inputPort),
      stream_configuration_(stream_configuration),
      clients_(clients) {
  stream_configuration_->setBool("streams.webSocket.supported", true);
  if (webstreamer_configuration->has("streams.webSocketStream.url")) {
    stream_configuration_->setString(
        "streams.webSocket.url",
        webstreamer_configuration->getString("streams.webSocketStream.url"));
  }
  stream_configuration_->setUInt("streams.webSocket.port", port());
}

void WebSocketStreamServer::OnConnect(const Poco::Net::WebSocket& web_socket) {

  const std::string address = web_socket.peerAddress ( ).host ( ).toString ( );

  if ( AccessManager::getInstance ( ).addressIsAllowed ( address ))
  {
    clients_->Insert<WebSocketStreamClient>(std::move(web_socket));
  }
}

}  // namespace webstreamer
