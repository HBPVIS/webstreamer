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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_RAW_ENCODER_FACTORY_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_RAW_ENCODER_FACTORY_HPP_

#include "webstreamer/encoder_factory.hpp"
#include "webstreamer/suppress_warnings.hpp"
SUPPRESS_WARNINGS_BEGIN
#include "Poco/Util/JSONConfiguration.h"
SUPPRESS_WARNINGS_END

namespace webstreamer {

class WEBSTREAMER_EXPORT RawEncoderFactory : public EncoderFactory {
 public:
  RawEncoderFactory(const Poco::Util::JSONConfiguration* configuration,
                    Poco::Util::JSONConfiguration* stream_config);

  std::unique_ptr<Encoder> CreateEncoder(
      const Poco::JSON::Object& configuration) override;

 private:
  const Poco::Util::JSONConfiguration* configuration_;
  Poco::Util::JSONConfiguration* stream_config_;
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_RAW_ENCODER_FACTORY_HPP_
