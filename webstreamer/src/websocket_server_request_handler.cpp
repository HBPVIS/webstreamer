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

#include "webstreamer/websocket_server_request_handler.hpp"
#include "webstreamer/websocket_server.hpp"
#pragma warning(push, 0)
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/WebSocket.h"
#pragma warning(pop)
#include "log.hpp"

namespace webstreamer {

WebSocketServerRequestHandler::WebSocketServerRequestHandler(
    WebSocketServer* server)
    : server_(server) {}

void WebSocketServerRequestHandler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) {
  try {
    server_->OnConnect(Poco::Net::WebSocket(request, response));
  } catch (const Poco::Net::WebSocketException& ex) {
    LOGI("Invalid HTTP upgrade request: ", ex.message());
  }
}

}  // namespace webstreamer
