# WIP
This is a work-in-progress game engine using OpenGL.
## Dependencies
### Build tools
- GCC or Clang
- GNU Make and pkg-config or CMake
### Libraries
- GLFW or SDL2
- libconfig
## Building
### Using make
`$ ./configure.sh` and `$ make`
### Using CMake (Linux)
```
mkdir build
cd build
cmake ..
make
```
### Using CMake (Windows)
Under Windows, I recommend building as a [CMake project in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
and managing dependencies using [vcpkg](https://github.com/Microsoft/vcpkg).
