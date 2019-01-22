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

#include "webstreamer/client_set.hpp"
#include <algorithm>
#include "log.hpp"
#include "webstreamer/client.hpp"
#include "webstreamer/codec_event.hpp"
#include "webstreamer/down_cast.hpp"
#include "webstreamer/encoding_pipeline.hpp"
#include "webstreamer/input_processor.hpp"
#include "webstreamer/stop_watch.hpp"

namespace webstreamer {

ClientSet::ClientSet(EncodingPipeline* encoding_pipeline)
    : encoding_pipeline_(encoding_pipeline),
      update_thread_(&ClientSet::UpdateThread, this) {}

void ClientSet::Insert(std::unique_ptr<Client> client) {
  std::lock_guard<std::mutex> lock(vector_access_mutex_);
  clients_.push_back(std::move(client));
  // A check whether the client is already in the set should
  // not be needed as there should never be more than one
  // unique_ptr pointing to an individual instance.
}

void ClientSet::UpdateClients() {
  std::lock_guard<std::mutex> lock(vector_access_mutex_);

  for (const auto& client : clients_) {
    if (client->is_alive()) {
      Codec new_codec;
      CodecOptions new_codec_options;
      if (client->HasRequestedNewCodec(&new_codec, &new_codec_options)) {
        encoding_pipeline_->DeregisterClient(client.get());
        if (encoding_pipeline_->RegisterClient(client.get(), new_codec,
                                               new_codec_options)) {
          client->SetNewCodec(new_codec, new_codec_options);
          LOGI("Client changed codec!");
        } else {
          LOGW("Failed to change codec!");
        }
      }
    }
    client->InsertEvents(&events_);
  }
  ProcessEvents();

  clients_.erase(
      std::remove_if(clients_.begin(), clients_.end(),
                     [this](const std::unique_ptr<Client>& client) {
                       if (!client->is_alive()) {
                         encoding_pipeline_->DeregisterClient(client.get());
                         if (input_client_ == client.get()) {
                           client->owns_input_token_ = false;
                           input_client_ = nullptr;
                         }
                         return true;
                       } else {
                         return false;
                       }
                     }),
      clients_.end());
}

void ClientSet::OnStreamConfigChanged() {
  std::lock_guard<std::mutex> lock(vector_access_mutex_);

  Event event(EventType::STREAM_CONFIG_CHANGED);
  for (const auto& client : clients_) {
    client->SendEvent(event);
  }
}

void ClientSet::SetInputProcessor(InputProcessor* input_processor) {
  std::lock_guard<std::mutex> lock(input_processor_mutex_);
  input_processor_ = input_processor;
}

void ClientSet::UpdateThread() {
  while (true) {
    UpdateClients();
    std::this_thread::yield();
  }
}

void ClientSet::ProcessEvents() {
  std::sort(events_.begin(), events_.end(),
            [](const ClientEvent& lhs, const ClientEvent& rhs) {
              return lhs.timestamp < rhs.timestamp;
            });

  for (auto& event : events_) {
    LOGI("Received Event: ", event.ptr->ToString());

    switch (event.ptr->type()) {
      case EventType::AQUIRE_INPUT:
        if (input_client_ != nullptr) {
          input_client_->owns_input_token_ = false;
        }
        input_client_ = event.client;
        input_client_->owns_input_token_ = true;
        break;

      case EventType::RELEASE_INPUT:
        if (input_client_ == event.client) {
          input_client_->owns_input_token_ = false;
          input_client_ = nullptr;
        }
        break;

      case EventType::PLAY:
        event.client->is_playing_ = true;
        break;

      case EventType::STOP:
        event.client->is_playing_ = false;
        break;

      case EventType::CHANGE_CODEC: {
        auto codec_event = down_cast<CodecEvent*>(event.ptr.get());
        event.client->SwitchCodec(codec_event->codec(), codec_event->options());
        break;
      }

      case EventType::MOUSE_INPUT:
        if (event.client->owns_input_token_ && input_processor_ != nullptr) {
          input_processor_->PushEvent(std::move(event.ptr));
        }
        break;

      case EventType::KEYBOARD_INPUT:
        if (event.client->owns_input_token_ && input_processor_ != nullptr) {
          input_processor_->PushEvent(std::move(event.ptr));
        }
        break;

      case EventType::STREAM_CONFIG_CHANGED:
        break;

      case EventType::UNKNOWN:
        LOGI("Unknown event type for client ", event.client);
        break;
    }
  }

  events_.clear();
}

}  // namespace webstreamer
