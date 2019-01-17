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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_REQUEST_HANDLER_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_REQUEST_HANDLER_HPP_

#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Util/JSONConfiguration.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

class WebServerRequestHandler : public Poco::Net::HTTPRequestHandler {
 public:
  WebServerRequestHandler(const Poco::Util::JSONConfiguration* stream_config,
                          const std::string& root_dir);
  void handleRequest(Poco::Net::HTTPServerRequest& request,
                     Poco::Net::HTTPServerResponse& response) override;

 private:
  const Poco::Util::JSONConfiguration* stream_config_;
  const std::string& root_dir_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_WEB_SERVER_REQUEST_HANDLER_HPP_
