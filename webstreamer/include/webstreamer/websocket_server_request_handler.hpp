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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_SERVER_REQUEST_HANDLER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_SERVER_REQUEST_HANDLER_HPP_

#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/Net/HTTPRequestHandler.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

class WebSocketServer;

class WebSocketServerRequestHandler : public Poco::Net::HTTPRequestHandler {
 public:
  WebSocketServerRequestHandler(WebSocketServer* server);

  void handleRequest(Poco::Net::HTTPServerRequest& request,
                     Poco::Net::HTTPServerResponse& response) override;

 private:
  WebSocketServer* server_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_REQUEST_HANDLER_HPP_
