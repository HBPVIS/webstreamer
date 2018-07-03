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

#include <cassert>
#include <webstreamer/client.hpp>
#include <webstreamer/encoding_pipeline.hpp>

namespace webstreamer {

void Client::PushFrame(const EncodedFrame& encoded_frame) {
  if (is_active()) {
    OnFrameEncoded(encoded_frame);
  }
}

void Client::SwitchCodec(Codec codec, CodecOptions options) {
  std::lock_guard<std::mutex> lock(requested_codec_mutex_);
  requested_codec_ = codec;
  requested_codec_options_ = options;
  requested_codec_new_codec_ = true;
}

void Client::Die() { is_alive_ = false; }

void Client::AddEvent(Event::Ptr event) {
  auto timestamp = std::chrono::steady_clock::now();
  if (is_alive()) {
    std::lock_guard<std::mutex> lock(events_mutex_);
    events_.push_back(ClientEvent{this, timestamp, std::move(event)});
  }
}

bool Client::HasRequestedNewCodec(Codec* codec, CodecOptions* options) {
  std::lock_guard<std::mutex> lock(requested_codec_mutex_);
  bool result = requested_codec_new_codec_;
  if (result) {
    requested_codec_new_codec_ = false;
  }
  if (codec != nullptr) {
    *codec = requested_codec_;
  }
  if (options != nullptr) {
    *options = requested_codec_options_;
  }
  return result;
}

void Client::SetNewCodec(Codec codec, const CodecOptions& options) {
  if (is_alive()) {
    OnCodecSwitched(codec, options);
  }
}

void Client::InsertEvents(std::vector<ClientEvent>* events) {
  assert(events != nullptr);
  std::lock_guard<std::mutex> lock(events_mutex_);
  events->reserve(events->size() + events_.size());
  for (auto& event : events_) {
    events->push_back(std::move(event));
  }
  events_.clear();
}

}  // namespace webstreamer
