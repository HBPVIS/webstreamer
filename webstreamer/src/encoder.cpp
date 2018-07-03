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

#include "webstreamer/encoder.hpp"
#include <algorithm>
#include <cassert>
#include "webstreamer/client.hpp"

namespace webstreamer {

Encoder::Encoder() : idle_time_(true) {}

void Encoder::RegisterClient(Client* client) {
  std::lock_guard<std::mutex> lock(clients_access_mutex_);
#ifndef NDEBUG
  for (auto c : clients_) {
    assert(c != client);
  }
#endif
  clients_.push_back(client);
  has_new_client_ = true;
  idle_time_.Stop();
  idle_time_.Reset();
}

void Encoder::DeregisterClient(Client* client) {
  std::lock_guard<std::mutex> lock(clients_access_mutex_);
  clients_.erase(std::remove(clients_.begin(), clients_.end(), client),
                 clients_.end());
  if (clients_.size() == 0) {
	  idle_time_.Start();
  }
}

void Encoder::PushFrame(const FrameBuffer& frame_buffer) {
  {
    std::lock_guard<std::mutex> lock(clients_access_mutex_);
    bool has_active_clients = false;
    for (const auto& client : clients_) {
      if (client->is_active()) {
        has_active_clients = true;
        break;
      }
    }
    if (!has_active_clients) {
      return;
    }
  }
  SendEncodedFrameToRegisteredClients(EncodeFrame(frame_buffer));
  has_new_client_ = false;
}

void Encoder::SendEncodedFrameToRegisteredClients(
    const EncodedFrame& encoded_frame) {
  std::lock_guard<std::mutex> lock(clients_access_mutex_);
  for (auto client : clients_) {
    client->PushFrame(encoded_frame);
  }
}

}  // namespace webstreamer
