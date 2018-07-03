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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_HPP_

#include <cstdint>
#pragma warning(push, 0)
#pragma warning(disable: 4702)
#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/JSONConfiguration.h"
#pragma warning(pop)

namespace webstreamer {

class WebServer : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  WebServer(const Poco::Util::JSONConfiguration* stream_config,
            const std::string& root_dir, std::uint16_t port = 80);

  Poco::Net::HTTPRequestHandler* createRequestHandler(
      const Poco::Net::HTTPServerRequest& request) override;

 private:
  const Poco::Util::JSONConfiguration* stream_config_;
  std::string root_dir_;
  Poco::Net::HTTPServer http_server_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_HPP_
