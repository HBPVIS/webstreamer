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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_CLIENT_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_CLIENT_HPP_

#include <cstdint>
#include <mutex>
#include <vector>
#include "webstreamer/encoder.hpp"
#include "webstreamer/event.hpp"
#include "webstreamer/export.hpp"

namespace webstreamer {

class EncodingPipeline;
struct EncodedFrame;

struct ClientEvent {
#if defined(_MSC_VER) && _MSC_VER < 1900
  // VS 2013 cannot generate the default move constructor and move assignment
  // operator
  ClientEvent(Client* client, std::chrono::steady_clock::time_point timestamp,
              Event::Ptr ptr)
      : client(client), timestamp(timestamp), ptr(std::move(ptr)) {}
  ClientEvent(ClientEvent&& other)
      : client(other.client),
        timestamp(other.timestamp),
        ptr(std::move(other.ptr)) {}
  ClientEvent& operator=(ClientEvent&& other) {
    client = other.client;
    timestamp = other.timestamp;
    ptr = std::move(other.ptr);
    return *this;
  }
#endif

  Client* client;
  std::chrono::steady_clock::time_point timestamp;
  Event::Ptr ptr;
};

class WEBSTREAMER_EXPORT Client {
  friend class ClientSet;

 public:
  virtual ~Client() = default;

  inline bool is_alive() const { return is_alive_; }
  inline bool is_playing() const { return is_playing_; }
  inline bool is_active() const { return is_alive() && is_playing(); }

  void PushFrame(const EncodedFrame& encoded_frame);

  virtual void OnFrameEncoded(const EncodedFrame& encoded_frame) = 0;
  virtual void OnCodecSwitched(Codec codec, const CodecOptions& options) = 0;
  virtual void SendEvent(const Event& event) = 0;
  inline void SendEvent(Event::Ptr event) { SendEvent(*event.get()); }

 protected:
  // The switch does not happen immediately. Wait for the corresponding call
  // to OnCodecSwitched(). Two consecutive calls to this function may only
  // cause one call to OnCodecSwitched() if the second calls occurs before
  // the actual switch.
  void SwitchCodec(Codec codec, CodecOptions options);

  // Call this function if the connection has been closed. The client will not
  // receive any updates and will be destroyed in the near future.
  void Die();

  // Should be called whenever the client receives an event.
  void AddEvent(Event::Ptr event);

 private:
  bool is_alive_ = true;
  bool is_playing_ = true;
  bool owns_input_token_ = false;

  std::mutex requested_codec_mutex_;
  bool requested_codec_new_codec_ = false;
  Codec requested_codec_;
  CodecOptions requested_codec_options_;

  std::mutex events_mutex_;
  std::vector<ClientEvent> events_;

  // Returns whether a new codec has been requested since the last call to
  // this functions.
  bool HasRequestedNewCodec(Codec* codec, CodecOptions* options);
  void SetNewCodec(Codec codec, const CodecOptions& options);
  void InsertEvents(std::vector<ClientEvent>* events);
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_CLIENT_HPP_
