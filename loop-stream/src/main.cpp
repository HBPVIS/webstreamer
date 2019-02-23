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

#include <png.h>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "webstreamer/console_logger.hpp"
#include "webstreamer/file_logger.hpp"
#include "webstreamer/stop_watch.hpp"
#include "webstreamer/suppress_warnings.hpp"
#include "webstreamer/webstreamer.hpp"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif

struct Image {
  int width;
  int height;
  std::vector<std::uint8_t> pixels;
};

bool read_png_file(const std::string& filename, Image* image);

int main(int argc, const char** argv) {
  using webstreamer::AsynchronousInputProcessor;
  using webstreamer::ConsoleLogger;
  using webstreamer::CreateImmortalLogListener;
  using webstreamer::DEFAULT_RESOLUTIONS;
  using webstreamer::FileLogger;
  using webstreamer::LogLevel;
  using webstreamer::Resolution;
  using webstreamer::StreamingProtocol;
  using webstreamer::WebStreamer;

  if (argc < 6) {
    std::cout << "usage: test-stream [image_format] [image_count] [width] "
                 "[height] [bitrate] [fps]"
              << std::endl;
    return -1;
  }

  const std::string& image_format = argv[1];
  const int image_count = std::stoi(argv[2]);
  const int width = std::stoi(argv[3]);
  const int height = std::stoi(argv[4]);
  const int bitrate = std::stoi(argv[5]);
  const int fps = std::stoi(argv[6]);
  char filename[1024];

  std::vector<Image> images(image_count);

  for (int i = 0; i < image_count; ++i) {
    snprintf(filename, sizeof(filename), image_format.c_str(), i + 1);
    if (!read_png_file(filename, &images[i])) {
      std::cerr << "Failed to load: " << filename << std::endl;
      return -1;
    } else {
      std::cout << "Loaded: " << filename << std::endl;
    }
  }

  CreateImmortalLogListener<ConsoleLogger>(LogLevel::DEBUG);
  CreateImmortalLogListener<FileLogger>(LogLevel::VERBOSE);
  AsynchronousInputProcessor input_processor;
  std::vector<Resolution> resolutions;
  if (width == 0 && height == 0) {
    resolutions = DEFAULT_RESOLUTIONS;
  } else {
    resolutions = {{width, height, bitrate, fps}};
  }

  WebStreamer web_streamer;
  web_streamer.RegisterInputProcessor(&input_processor);

  const auto frame_interval =
      fps == 0 ? std::chrono::microseconds(0)
               : std::chrono::microseconds(static_cast<unsigned int>(
                     1000.0 * 1000.0 / static_cast<double>(fps)));

  webstreamer::StopWatch<> frame_stopwatch(true);
  int current_image_index = 0;
  while (true) {
    if (frame_stopwatch.elapsed_time() < frame_interval) {
      continue;
    }

    frame_stopwatch.Subtract(frame_interval);

    web_streamer.PushFrame(images[current_image_index].width,
                           images[current_image_index].height,
                           images[current_image_index].pixels.data(),
                           images[current_image_index].pixels.size());

    current_image_index = (current_image_index + 1) % image_count;
  }

  return 0;
}

SUPPRESS_WARNINGS_BEGIN
bool read_png_file(const std::string& filename, Image* image) {
  if (image == nullptr) {
    std::cout << "read_png_file: invalid image pointer" << std::endl;
    return false;
  }

  std::FILE* fp = std::fopen(filename.c_str(), "rb");
  if (!fp) {
    std::cout << "read_png_file: cannot open file" << std::endl;
    return false;
  }

  png_byte header[8];
  auto r = fread(header, 1, 8, fp);
  if ( r != 1 ) {
    std::cerr << "read_png_file: error reading header" << std::endl;
    return false;
  }
  if (png_sig_cmp(header, 0, 8)) {
    std::cout << "read_png_file: invalid header" << std::endl;
    return false;
  }

  png_structp png_ptr =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_ptr) {
    std::cout << "read_png_file: cannot create read struct" << std::endl;
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    std::cout << "read_png_file: cannot create info struct" << std::endl;
    return false;
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cout << "read_png_file: cannot setjmp" << std::endl;
    return false;
  }

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
  png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  if (color_type != PNG_COLOR_TYPE_RGB) {
    std::cout << "read_png_file: invalid color type" << std::endl;
    return false;
  }

  if (bit_depth != 8) {
    std::cout << "read_png_file: invalid bit depth" << std::endl;
    return false;
  }

  /*int number_of_passes = */ png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr))) {
    std::cout << "read_png_file: cannot setjmp" << std::endl;
    return false;
  }

  image->width = static_cast<int>(width);
  image->height = static_cast<int>(height);
  image->pixels.resize(width * height * 3);
  if (png_get_rowbytes(png_ptr, info_ptr) != width * 3) {
    std::cout << "read_png_file: invalid num bytes per row" << std::endl;
    return false;
  }
  std::vector<png_bytep> row_pointers(height, nullptr);
  for (png_uint_32 y = 0; y < height; ++y) {
    row_pointers[y] = image->pixels.data() + y * width * 3;
  }

  png_read_image(png_ptr, row_pointers.data());

  fclose(fp);

  return true;
}
SUPPRESS_WARNINGS_END
