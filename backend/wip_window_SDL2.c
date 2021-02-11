//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// SDL2 Window Functions

#include <pthread.h>
#include <SDL2/SDL.h>

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_window.h"
#include "wip_input.h"

wip_window_t wip_globalWindow;
pthread_mutex_t wip_globalWindow_m;

// TODO: load config values
void wip_initWindow(void) {
	wip_debug(WIP_INFO, "%s: Initializing window...", __func__);

	if(SDL_Init(SDL_INIT_VIDEO))
		wip_log(WIP_FATAL, "%s: Couldn't initialize SDL2.", __func__);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, wip_getConfInt("graphics.msaa") > 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, wip_getConfInt("graphics.msaa"));

	wip_globalWindow.handle = SDL_CreateWindow("WIP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		wip_getConfInt("video.width"), wip_getConfInt("video.height"), SDL_WINDOW_OPENGL);

	if(!wip_globalWindow.handle) {
		wip_log(WIP_FATAL, "%s: Couldn't create window.", __func__);
	}
	SDL_GL_CreateContext(wip_globalWindow.handle);
	SDL_GL_SetSwapInterval(wip_getConfBool("video.vsync"));

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
}

void wip_setWindow(void) {
	SDL_GL_CreateContext(wip_globalWindow.handle);
}

void wip_swapWindow(void) {
	SDL_GL_SwapWindow(wip_globalWindow.handle);
}

// TODO: input
void wip_pollWindow(void) {
	SDL_Event event;
	SDL_PollEvent(&event);
	if(event.type == SDL_QUIT) wip_globalWindow.close = 1;
}

double wip_timeWindow(void) {
	return SDL_GetTicks()/1000.0;
}

// TODO: if needed better cleanup
void wip_termWindow(void) {
	wip_debug(WIP_INFO, "%s: Terminating window...", __func__);
	SDL_Quit();
}

