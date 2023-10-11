# WIP
![image](https://user-images.githubusercontent.com/21199271/178315181-3fda2625-7027-4096-8e45-a440ebda88ac.png)
This is a work-in-progress game engine using OpenGL and a game called Dungeon implemented in it.
## Dependencies
### Build tools
- GCC or Clang
- GNU Make and pkg-config or CMake
### Libraries
- GLFW, SDL2 or OSMesa
- libconfig
## Building
### Using make
```
$ ./configure.sh
$ make
```
### Using CMake (Linux)
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
### Using CMake (Windows)
Under Windows, I recommend building as a
[CMake project in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
and managing dependencies using
[vcpkg](https://github.com/Microsoft/vcpkg).
