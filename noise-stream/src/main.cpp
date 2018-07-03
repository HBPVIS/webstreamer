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

#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <random>
#include <vector>
#include "webstreamer/console_logger.hpp"
#include "webstreamer/file_logger.hpp"
#include "webstreamer/stop_watch.hpp"
#include "webstreamer/webstreamer.hpp"

int main(int argc, const char** argv) {
  using webstreamer::ConsoleLogger;
  using webstreamer::CreateImmortalLogListener;
  using webstreamer::FileLogger;
  using webstreamer::LogLevel;
  using webstreamer::WebStreamer;

  if (argc >= 2 &&
      (std::strcmp(argv[1], "--help") == 0 ||
       std::strcmp(argv[1], "help") == 0 || std::strcmp(argv[1], "-h") == 0)) {
    std::cout << "usage: noise-stream [width=1280] [height=720] [fps=60]"
              << std::endl;
    return 0;
  }

  const int width = argc >= 2 ? std::atoi(argv[1]) : 1280;
  const int height = argc >= 3 ? std::atoi(argv[2]) : 720;
  const int fps = argc >= 4 ? std::atoi(argv[3]) : 60;
  const int bytes_per_line = width * 3;
  const int stride = ((bytes_per_line + 3) / 4) * 4;

  std::vector<std::uint8_t> image(stride * height);
  std::uint32_t* const u32_image_values =
      reinterpret_cast<std::uint32_t*>(image.data());
  const std::size_t u32_image_values_count = image.size() / 4;
  assert(image.size() % 4 == 0);

  CreateImmortalLogListener<ConsoleLogger>(LogLevel::DEBUG);
  CreateImmortalLogListener<FileLogger>(LogLevel::VERBOSE);

  WebStreamer web_streamer;

  class InputProcessor : public webstreamer::AsynchronousInputProcessor {
  public:
	  void ProcessMouseInput(const webstreamer::MouseEvent&) override {
		  webstreamer::LogI("Process mouse input...");
	  }
	  void ProcessKeyboardInput(const webstreamer::KeyboardEvent&) override {
		  webstreamer::LogI("Process keyboard input...");
	  }
  };
  InputProcessor input_processor;
  web_streamer.RegisterInputProcessor(&input_processor);

  const auto frame_interval =
      fps == 0 ? std::chrono::microseconds(0)
               : std::chrono::microseconds(static_cast<unsigned int>(
                     1000.0 * 1000.0 / static_cast<double>(fps)));

  webstreamer::StopWatch<> frame_stopwatch(true);
  // webstreamer::StopWatch<> fps_watch(true);
  // unsigned int frame_counter = 0;

  std::uniform_int_distribution<std::uint32_t> distribution;
  std::mt19937 random_number_generator_engine;

  while (true) {
    // if (fps_watch.elapsed_time() >= std::chrono::seconds(1)) {
    //   std::cout << '\r' << frame_counter << " fps";
    //   frame_counter = 0;
    //   fps_watch.Subtract(std::chrono::seconds(1));
    // }

    if (frame_stopwatch.elapsed_time() < frame_interval) {
      continue;
    }
    frame_stopwatch.Subtract(frame_interval);

    for (std::size_t i = 0; i < u32_image_values_count; ++i) {
      u32_image_values[i] = distribution(random_number_generator_engine);
    }
    web_streamer.PushFrame(width, height, image.data(), image.size());
    // ++frame_counter;
  }

  return 0;
}
