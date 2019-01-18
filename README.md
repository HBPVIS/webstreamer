# webstreamer

*Web Streamer* is Copyright (c) 2017 RWTH Aachen University, Germany,
Virtual Reality & Immersive Visualization Group.

## Prerequisites

* *CMake* >= 3.1 (https://cmake.org/)

## Dependencies

* [POCO](https://github.com/pocoproject/poco)
* [libswscale](https://ffmpeg.org/libswscale.html)
* [x264](https://www.videolan.org/developers/x264.html)
* [WebRTC](https://webrtc.org/) (optional)
* [libpng](http://www.libpng.org/) (for tests)

On Ubuntu you can install POCO, libswscale, x264 and libpng through the package manager:
```
sudo apt install libpoco-dev
sudo apt install libx264-dev
sudo apt install libswscale-dev
sudo apt install libpng-dev
```

Precompiled versions of WebRTC can be found [here](https://sourcey.com/precompiled-webrtc-libraries).

## Building

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `cmake --build .`

## Test

To test if everything is working, run the *noise-stream* example. This requires a proper configuration file called `webstreamer_config.json` in the working directory (see next section). Type in `http://localhost:<PORT>/index.html` in a browser (replace `<PORT>` with the port specified in the configuration file) and you should see a video stream with random pixel data. Clicking on the *play/pause* icon will pause or resume the stream. Clicking on the *pencil* icon will gain control over the underlying application, i.e., the mouse and keyboard input is now forwarded to the underlying application (which, however, has no effect in this example).

## Configuration File

WebStreamer uses a configuration file in order to change its settings. An example of such a configuration can be found in the root directory of the repository and is called `webstreamer_config.json`. 

## Integration

After building the library via _Cmake_ as described in the previous section the library has to be itegrated in the target application. If the target application also uses the _CMake_ build system the integration process is very straight forward. When executing _CMake_ pass the additional flag `-Dwebstreamer_DIR=<DIR>` where `<DIR>` points to the build directory of the library, i.e., the directory that contains the _webstreamer-config.cmake_ file. Then, the necessary include directories and libraries can be added to existing targets as follows:

```cmake
target_include_directories(<TARGET> PUBLIC webstreamer)
target_link_libraries(<TARGET> webstreamer)
```

All include files are contained in the folder _webstreamer_ and the namespace with the same name is used to encapsulate all classes and functions. The main class of the library is the _WebStreamer_ class that is defined in the header file _webstreamer/webstreamer.hpp_. This class must be instantiated by the application in order to use the library. Its constructor takes the path to a configuration file that can be used to define the behaviour of the library. In addition, there are two distinct interaction points between the application and the library: passing video ouput from the application to the library and passing input events from the library to the server. All three topics are covered in the following.


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

Handling input data on the server side is done using the `InputProcessor` interface. Which must be registered to the main `WebStreamer` class using its `RegisterInputProcessor()` method. The application should either derive from the `SynchronousInputProcessor` or `AsynchronousInputProcessor` class and implement the `ProcessMouseInput()` and `ProcessKeyboardInput()` functions accordingly. The difference between the two classes lies in the exact time the member functions are called. In the case of the `AsynchronousInputProcessor` the corresponding function is called from a seperate thread immediately when an input event is received at the server side. The `SynchronousInputProcessor` on the other hand buffers all events and calls the corresponding function only if the application calls its `ProcessInput()` function. The repository contains an example implementation of an `AsynchronousInputProcessor` for Qt applications. This can be found in the *qt_inputprocessor* subdirectory. If you are developing a Qt application you can use this implementation directly (make sure to pass the flag `-DBUILD_QT_INPUTPROCESSOR=ON` to the cmake command line).

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
