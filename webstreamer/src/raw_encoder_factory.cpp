#include "webstreamer/raw_encoder_factory.hpp"
#include "webstreamer/raw_encoder.hpp"

namespace webstreamer {

RawEncoderFactory::RawEncoderFactory(
    const Poco::Util::JSONConfiguration* configuration,
    Poco::Util::JSONConfiguration* stream_config)
    : configuration_(configuration), stream_config_(stream_config) {
  stream_config_->setBool("codecs.raw.supported", true);
}

std::unique_ptr<Encoder> RawEncoderFactory::CreateEncoder(
    const Poco::JSON::Object& configuration) {
  (void)configuration;
  return std::make_unique<RawEncoder>();
}

}  // namespace webstreamer