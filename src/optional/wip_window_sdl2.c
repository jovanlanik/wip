// WIP
// Copyright (c) 2020 Jovan Lanik

// SDL2 Window Functions

#include <SDL2/SDL.h>

#include "wip_fn.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_conf.h"

wip_window_t wip_globalWindow;

int unifyKey(SDL_Keysym key) {
	switch(key.scancode) {
		case SDL_SCANCODE_UNKNOWN: return WIP_UNKNOWN;
		case SDL_SCANCODE_NUMLOCKCLEAR: return WIP_KP_NUM_LOCK;
		case SDL_SCANCODE_KP_ENTER: return WIP_KP_ENTER;
		case SDL_SCANCODE_BACKSPACE: return WIP_BACKSPACE;
		case SDL_SCANCODE_TAB: return WIP_TAB;
		case SDL_SCANCODE_PRINTSCREEN: return WIP_PRINT_SCREEN;
		case SDL_SCANCODE_PAUSE: return WIP_PAUSE;
		case SDL_SCANCODE_RETURN: return WIP_ENTER;
		case SDL_SCANCODE_LCTRL: return WIP_L_CTRL;
		case SDL_SCANCODE_LSHIFT: return WIP_L_SHIFT;
		case SDL_SCANCODE_LGUI: return WIP_L_GUI;
		case SDL_SCANCODE_LALT: return WIP_L_ALT;
		case SDL_SCANCODE_RCTRL: return WIP_R_CTRL;
		case SDL_SCANCODE_RSHIFT: return WIP_R_SHIFT;
		case SDL_SCANCODE_RGUI: return WIP_R_GUI;
		case SDL_SCANCODE_RALT: return WIP_R_ALT;
		case SDL_SCANCODE_INSERT: return WIP_INSERT;
		case SDL_SCANCODE_HOME: return WIP_HOME;
		case SDL_SCANCODE_END: return WIP_END;
		case SDL_SCANCODE_PAGEUP: return WIP_PAGEUP;
		case SDL_SCANCODE_PAGEDOWN: return WIP_PAGEDOWN;
		case SDL_SCANCODE_ESCAPE: return WIP_ESCAPE;
		case SDL_SCANCODE_RIGHT: return WIP_RIGHT;
		case SDL_SCANCODE_LEFT: return WIP_LEFT;
		case SDL_SCANCODE_DOWN: return WIP_DOWN;
		case SDL_SCANCODE_UP: return WIP_UP;
		case SDL_SCANCODE_SPACE: return WIP_SPACE;
		case SDL_SCANCODE_CAPSLOCK: return WIP_CAPS_LOCK;
		case SDL_SCANCODE_MENU: return WIP_MENU;
		case SDL_SCANCODE_DELETE: return WIP_DELETE;
		default: break;
	}
	if(key.scancode == SDL_SCANCODE_0) return WIP_NUM;
	if(key.scancode >= SDL_SCANCODE_1 && key.scancode <= SDL_SCANCODE_9) return key.scancode - SDL_SCANCODE_1 + WIP_NUM+1;
	if(key.scancode >= SDL_SCANCODE_A && key.scancode <= SDL_SCANCODE_Z) return key.scancode + ('a' - SDL_SCANCODE_A);
	if(key.scancode >= SDL_SCANCODE_F1 && key.scancode <= SDL_SCANCODE_F12) return key.scancode - SDL_SCANCODE_F1 + WIP_F+1;
	if(key.scancode >= SDL_SCANCODE_F13 && key.scancode <= SDL_SCANCODE_F24) return key.scancode - SDL_SCANCODE_F13 + WIP_F+13;
	wip_log(WIP_WARN, "SDL2: Couldn't unify scancode: %d", key.scancode);
	return WIP_UNKNOWN;
}


// TODO: fix vsync (I don't remember what's broken, so I'll have to find out first...)
// TODO: check msaa before setting and set closest possible value
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

	SDL_GL_CreateContext(wip_globalWindow.handle);

	//SDL_GL_CreateContext(wip_globalWindow.handle);
	SDL_GL_SetSwapInterval(wip_getConfBool("video.vsync"));

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
}

void wip_swapWindow(void) {
	SDL_GL_SwapWindow(wip_globalWindow.handle);
}

// TODO: input (I think I did this? I'm afraid to remove this comment in case I meant something else...)
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

// TODO: if needed, better cleanup
void wip_termWindow(void) {
	wip_debug(WIP_INFO, "%s: Terminating window...", __func__);
	SDL_Quit();
}

