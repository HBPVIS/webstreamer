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
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include "webstreamer/webstreamer.hpp"

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif

using webstreamer::WebStreamer;
using webstreamer::Codec;
using webstreamer::StreamingProtocol;

struct Image {
  int width;
  int height;
  std::vector<std::uint8_t> pixels;
};

bool read_png_file(const std::string& filename, Image* image);

int main(int argc, const char** argv) {
  if (argc < 4) {
    std::cout
      << "usage: test-stream [image_format] [image_count] [out_filename]"
      << std::endl;
    return -1;
  }

  const std::string& image_format = argv[1];
  const int image_count = std::stoi(argv[2]);
  // const std::string& out_filename = argv[3];
  char filename[1024];

  WebStreamer web_streamer;

  Image image;
  for (int i = 1; i <= image_count; ++i) {
    snprintf(filename, sizeof(filename), image_format.c_str(), i);
    if (!read_png_file(filename, &image)) {
      std::cout << "Failed to load: " << filename << std::endl;
    } else {
      web_streamer.PushFrame(image.width, image.height, image.pixels.data(),
                             image.pixels.size());
      std::cout << "Finished frame " << i << " of " << image_count << "\n";
    }
  }

  return 0;
}

#pragma warning(disable:4611)
bool read_png_file(const std::string& filename, Image* image) {
  if (image == nullptr) {
    std::cout << "read_png_file: invalid image pointer" << std::endl;
    return false;
  }

  std::FILE *fp = std::fopen(filename.c_str(), "rb");
  if (!fp) {
    std::cout << "read_png_file: cannot open file" << std::endl;
    return false;
  }

  png_byte header[8];
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8)) {
    std::cout << "read_png_file: invalid header" << std::endl;
    return false;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
    nullptr,
    nullptr,
    nullptr);
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

  /*int number_of_passes = */png_set_interlace_handling(png_ptr);
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
#pragma warning(default:4611)
