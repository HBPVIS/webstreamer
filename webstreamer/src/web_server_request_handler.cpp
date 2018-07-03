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

#include "webstreamer/web_server_request_handler.hpp"
#include <fstream>
#pragma warning(push, 0)
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#pragma warning(pop)
#include "log.hpp"

namespace webstreamer {

WebServerRequestHandler::WebServerRequestHandler(
    const Poco::Util::JSONConfiguration* stream_config,
    const std::string& root_dir)
    : stream_config_(stream_config), root_dir_(root_dir) {}

void WebServerRequestHandler::handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) {
  (void)request;
  (void)response;

  if (request.getURI() == "/stream_config.json") {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    stream_config_->save(response.send());
  } else {
    std::ifstream file(root_dir_ + request.getURI(), std::ios::binary);
    if (!file.is_open()) {
      LOGI("Request ", request.getURI(), ": not found!");
      response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    } else {
      LOGI("Request ", request.getURI());
      response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
      const std::string extension = Poco::Path(request.getURI()).getExtension();
      if (extension == "html" || extension == "htm") {
        response.setContentType("text/html");
      } else if (extension == "js") {
        response.setContentType("text/javascript");
      } else if (extension == "css") {
        response.setContentType("text/css");
      } else if (extension == "gif") {
        response.setContentType("image/gif");
      } else if (extension == "jpg" || extension == "jpeg") {
        response.setContentType("image/jpeg");
      } else if (extension == "png") {
        response.setContentType("image/png");
      } else if (extension == "svg") {
        response.setContentType("image/svg+xml");
      } else {
        LOGD("Unknown content type");
      }
    }
    Poco::StreamCopier::copyStream(file, response.send());
  }
}

}  // namespace webstreamer
