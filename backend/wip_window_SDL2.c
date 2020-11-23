//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// SDL2 Window Functions

#include "SDL2/SDL.h"

#include "wip_fn.h"
#include "wip_window.h"
#include "wip_input.h"

void wip_initWindow(wip_window_t *window) {
	wip_debug(WIP_INFO, "%s: Initializing window...", __func__);

	if(SDL_Init(SDL_INIT_VIDEO))
		wip_log(WIP_FATAL, "%s: Couldn't initialize SDL2.", __func__);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	window->handle = SDL_CreateWindow("WIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1680, 1050, SDL_WINDOW_OPENGL);
	if(!window->handle) {
		wip_log(WIP_FATAL, "%s: Couldn't create window.", __func__);
	}
	SDL_GL_SetSwapInterval(1);
	SDL_GL_CreateContext(window->handle);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
}

void wip_setWindow(wip_window_t *window) {
	SDL_GL_CreateContext(window->handle);
}

void wip_swapWindow(wip_window_t *window) {
	SDL_GL_SwapWindow(window->handle);
}

// TODO: input
void wip_pollWindow(wip_window_t *window) {
	SDL_Event event;
	SDL_PollEvent(&event);
	if(event.type == SDL_QUIT) window->close = 1;
}

double wip_timeWindow(wip_window_t *window) {
	return SDL_GetTicks()/1000.0;
}

// TODO: if needed better cleanup
void wip_termWindow(wip_window_t *window) {
	wip_debug(WIP_INFO, "%s: Terminating window...", __func__);
	SDL_Quit();
}

