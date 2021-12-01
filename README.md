# WIP
This is work in progress.
## Dependencies
- GLEW
- GLFW or SDL2
- libconfig
- xxd (at build time)
- bear (if you want to use clangd)
## Building
`$ ./configure.sh` and `$ make`
or if you want to use the SDL2 backend:
`$ ./configure.sh WINDOW_BACKEND=SDL2` and `$ make`

SDL2 backend may not work correctly.
