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

#ifndef WEBSTREAMER_INCLUDE_WEBSTREAMER_CLIENT_SET_HPP_
#define WEBSTREAMER_INCLUDE_WEBSTREAMER_CLIENT_SET_HPP_

#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>
#include "webstreamer/client.hpp"
#include "webstreamer/export.hpp"

namespace webstreamer {

class EncodingPipeline;
class InputProcessor;

class WEBSTREAMER_EXPORT ClientSet {
 public:
  explicit ClientSet(EncodingPipeline* encoding_pipeline);

  template <typename T, typename... Args>
  void Insert(Args&&... constructor_arguments) {
    static_assert(std::is_base_of<Client, T>::value,
                  "T must be a dervied class of Client");
    static_assert(std::is_constructible<T, Args...>::value,
                  "The class is not constructible with the given arguments");
    std::lock_guard<std::mutex> lock(vector_access_mutex_);
    clients_.emplace_back(
        std::make_unique<T>(std::forward<Args>(constructor_arguments)...));
  }
  void Insert(std::unique_ptr<Client> client);
  void UpdateClients();
  void OnStreamConfigChanged();
  void SetInputProcessor(InputProcessor* input_processor);

 private:
  EncodingPipeline* encoding_pipeline_;
  std::vector<std::unique_ptr<Client>> clients_;
  Client* input_client_ = nullptr;
  std::mutex vector_access_mutex_;
  std::vector<ClientEvent> events_;

  std::thread update_thread_;

  std::mutex input_processor_mutex_;
  InputProcessor* input_processor_;

  void UpdateThread();
  void ProcessEvents();
};

}  // namespace webstreamer

#endif  // WEBSTREAMER_INCLUDE_WEBSTREAMER_CLIENT_SET_HPP_
