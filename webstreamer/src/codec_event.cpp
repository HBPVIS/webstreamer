#include "webstreamer/codec_event.hpp"
#include <cassert>
#include <cstring>
#include <sstream>
#include "log.hpp"

#pragma warning(push, 0)
#include "Poco/JSON/Parser.h"
#pragma warning(pop)

namespace webstreamer {

CodecEvent::CodecEvent(const void* data, std::size_t size_in_bytes)
    : Event(EventType::CHANGE_CODEC) {
  const std::uint8_t* bytes = reinterpret_cast<const std::uint8_t*>(data);
  assert(static_cast<EventType>(bytes[0]) == type());
  codec_ = static_cast<Codec>(bytes[1]);
  const std::string options_string =
      std::string(reinterpret_cast<const char*>(data) + 2, size_in_bytes - 2);
  Poco::JSON::Parser parser;
  try {
    auto result = parser.parse(options_string);
    options_ = *result.extract<CodecOptions::Ptr>();
  } catch (const Poco::Exception& exception) {
    LOGW("Failed to parse codec options: ", exception.message());
  }
}

std::vector<std::uint8_t> CodecEvent::Serialize() const {
  const std::string options_string = CreateOptionsString();

  std::vector<std::uint8_t> buffer(2 + options_string.size());
  buffer[0] = static_cast<std::uint8_t>(type());
  buffer[1] = static_cast<std::uint8_t>(codec_);
  memcpy(buffer.data() + 2, options_string.c_str(), options_string.size());

  return buffer;
}

std::string CodecEvent::ToString() const {
  std::string codec_string;
  switch (codec()) {
    case Codec::RAW:
      codec_string = "Raw";
      break;

    case Codec::H264:
      codec_string = "H.264";
      break;
  }

  return "CHANGE_CODEC(" + codec_string + "," + CreateOptionsString() + ")";
}

std::string CodecEvent::CreateOptionsString() const {
  std::stringstream stream;
  options_.stringify(stream);
  return stream.str();
}

}  // namespace webstreamer
