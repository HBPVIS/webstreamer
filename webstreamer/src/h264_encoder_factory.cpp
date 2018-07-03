#include "webstreamer/h264_encoder_factory.hpp"
#include "webstreamer/h264_encoder.hpp"

namespace webstreamer {

H264EncoderFactory::H264EncoderFactory(
    const Poco::Util::JSONConfiguration* configuration,
    Poco::Util::JSONConfiguration* stream_config)
    : configuration_(configuration), stream_config_(stream_config) {
  if (configuration->getBool("codecs.h264.enabled")) {
    stream_config_->setBool("codecs.h264.supported", true);

    std::string configuration_key;
    std::string stream_config_key;
    for (int i = 0; configuration->has("codecs.h264.displayModes[" +
                                       std::to_string(i) + "]");
         ++i) {
      configuration_key = "codecs.h264.displayModes[" + std::to_string(i) + "]";
      stream_config_key =
          "codecs.h264.availableDisplayModes[" + std::to_string(i) + "]";

      stream_config->setInt(
          stream_config_key + ".width",
          configuration->getInt(configuration_key + ".width"));
      stream_config->setInt(
          stream_config_key + ".height",
          configuration->getInt(configuration_key + ".height"));
      stream_config->setInt(
          stream_config_key + ".framerate",
          configuration->getInt(configuration_key + ".framerate"));
    }
  }
}

std::unique_ptr<Encoder> H264EncoderFactory::CreateEncoder(
    const Poco::JSON::Object& options) {
  return std::make_unique<H264Encoder>(
      options.getValue<int>("width"), options.getValue<int>("height"),
      options.getValue<int>("framerate"), 6000);
}

}  // namespace webstreamer
