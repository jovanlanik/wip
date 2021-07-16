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

int unifyKey(SDL_Keysym key) {
	wip_log(
		WIP_INFO, "scancode: %d, keycode: %d, scanname: %s, keyname: %s",
		key.scancode,
		key.sym,
		SDL_GetScancodeName(key.scancode),
		SDL_GetKeyName(key.sym)
	);
	switch(key.scancode) {
		case SDL_SCANCODE_UNKNOWN: return WIP_UNKNOWN;
	}
	if(key.scancode >= SDL_SCANCODE_A && key.scancode <= SDL_SCANCODE_Z) return key.scancode + ('a' - SDL_SCANCODE_A);
	return WIP_UNKNOWN;
}


// TODO: fix vsync
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
		wip_log(WIP_FATAL, "%s: Couldn't create window: %s", __func__, SDL_GetError());
	}

	//SDL_GL_CreateContext(wip_globalWindow.handle);
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
	static SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				wip_debug(WIP_INFO, "SDL2: Quit event received...");
				wip_globalWindow.close = 1;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				if(event.key.repeat) return;
				wip_key_t nkey = {
					event.key.type == SDL_KEYDOWN ? WIP_PRESS : WIP_RELEASE,
					unifyKey(event.key.keysym)
				};
				if(!wip_writeKey(nkey)) wip_log(WIP_WARN, "SDL2: Dropped key event");
				break;
		}
	}
}

double wip_timeWindow(void) {
	return SDL_GetTicks()/1000.0;
}

// TODO: if needed better cleanup
void wip_termWindow(void) {
	wip_debug(WIP_INFO, "%s: Terminating window...", __func__);
	SDL_Quit();
}

