# WIP
This is work in progress.
It includes a demo and a small game.
## Dependencies
- GLEW
- GLFW or SDL2
- libconfig
- xxd (at build time)
## Building
`$ ./configure` and `$ make`
or if you want to use the SDL2 backend:
`$ ./configure WINDOW_BACKEND=SDL2` and `$ make`

Note that the SDL2 backend is behind GLFW and may not work correctly.
