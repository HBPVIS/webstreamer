# webstreamer

*Web Streamer* is Copyright (c) 2017 RWTH Aachen University, Germany,
Virtual Reality & Immersive Visualization Group.

## Dependencies

*Web Streamer* uses conan to manage it's external depencies. All dependencies are downloaded automatically while running cmake.

## Prerequisites

* *CMake* >= 3.1 (https://cmake.org/)
* *Conan*: `pip install conan`

## Building

1. Clone repository
2. Create build directory
3. Run cmake
4. Build library

## Integration

After building the library via _Cmake_ as described in the previous section the library has to be itegrated in the target application. If the target application also uses the _CMake_ build system the integration process is very straight forward. When executing _CMake_ pass the additional flag `-Dwebstreamer_DIR=<DIR>` where `<DIR>` points to the build directory of the library, i.e., the directory that contains the _webstreamer-config.cmake_ file. Then, the necessary include directories and libraries can be added to existing targets as follows:

```cmake
target_include_directories(<TARGET> PUBLIC webstreamer)
target_link_libraries(<TARGET> webstreamer)
```

All include files are contained in the folder _webstreamer_ and the namespace with the same name is used to encapsulate all classes and functions. The main class of the library is the _WebStreamer_ class that is defined in the header file _webstreamer/webstreamer.hpp_. This class must be instantiated by the application in order to use the library. Its constructor takes the path to a configuration file that can be used to define the behaviour of the library. In addition, there are two distinct interaction points between the application and the library: passing video ouput from the application to the library and passing input events from the library to the server. All three topics are covered in the following.

### Configuration File



### Sending Video data

A snapshot of the graphical output of the application is called a frame. Each time the graphical output of the application changes a new frame is produced. The application is responsible to get the content of each frame and pass it to the library. This is done using the _PushFrame_ member function of the _WebStreamer_ class:

```C++
void WebStreamer::PushFrame(
    std::size_t width,
    std::size_t height,
    const void* rgb_data,
    std::size_t size_in_bytes)
```

The parameters _width_ and _height_ correspond to the size of the frame. The size can vary arbitrarily between multiple calls. The library will take care of scaling and cropping the data correctly.

### Receiving Input Data



## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
