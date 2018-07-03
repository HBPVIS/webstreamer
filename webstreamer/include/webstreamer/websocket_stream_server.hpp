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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_STREAM_SERVER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_STREAM_SERVER_HPP_

#include "websocket_server.hpp"
#pragma warning(push, 0)
#include "Poco/Util/JSONConfiguration.h"
#pragma warning(pop)

namespace webstreamer {

class ClientSet;

class WEBSTREAMER_EXPORT WebSocketStreamServer : public WebSocketServer {
 public:
  WebSocketStreamServer(
      const Poco::Util::JSONConfiguration* webstreamer_configuration,
      Poco::Util::JSONConfiguration* stream_configuration, ClientSet* clients);

  void OnConnect(const Poco::Net::WebSocket& web_socket) override;

 private:
  const Poco::Util::JSONConfiguration* webstreamer_configuration_;
  Poco::Util::JSONConfiguration* stream_configuration_;
  ClientSet* clients_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_STREAM_HPP_
