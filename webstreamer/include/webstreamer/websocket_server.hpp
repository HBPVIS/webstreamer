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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_SERVER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_SERVER_HPP_

#include <cstdint>
#include "webstreamer/export.hpp"
#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/WebSocket.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

class WEBSTREAMER_EXPORT WebSocketServer
    : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  WebSocketServer(std::uint16_t port);

  Poco::Net::HTTPRequestHandler* createRequestHandler(
      const Poco::Net::HTTPServerRequest& request) override;

  virtual void OnConnect(const Poco::Net::WebSocket& web_socket) {
    (void)web_socket;
  }

  inline std::uint16_t port() const { return http_server_.port(); }

 private:
  Poco::Net::HTTPServer http_server_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEBSOCKET_SERVER_HPP_
