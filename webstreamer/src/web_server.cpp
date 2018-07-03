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

#include "webstreamer/web_server.hpp"
#include "log.hpp"
#include "webstreamer/web_server_request_handler.hpp"

namespace webstreamer {

WebServer::WebServer(const Poco::Util::JSONConfiguration* stream_config,
                     const std::string& root_dir, std::uint16_t port)
    : stream_config_(stream_config),
      root_dir_(root_dir),
      http_server_(this, port) {
  http_server_.start();
  LOGI("Started web server on port ", port);
}

Poco::Net::HTTPRequestHandler* WebServer::createRequestHandler(
    const Poco::Net::HTTPServerRequest&) {
  return new WebServerRequestHandler(stream_config_, root_dir_);
}

}  // namespace webstreamer
