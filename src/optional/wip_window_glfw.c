// WIP
// Copyright (c) 2020 Jovan Lanik

// GLFW Window Functions

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "wip_fn.h"
#include "wip_window.h"
#include "wip_gl.h"
#include "wip_input.h"
#include "wip_conf.h"

wip_window_t wip_globalWindow;

int unifyKey(int key) {
	switch(key) {
		case GLFW_KEY_UNKNOWN: return WIP_UNKNOWN;
		case GLFW_KEY_NUM_LOCK: return WIP_KP_NUM_LOCK;
		case GLFW_KEY_KP_ENTER: return WIP_KP_ENTER;
		case GLFW_KEY_BACKSPACE: return WIP_BACKSPACE;
		case GLFW_KEY_TAB: return WIP_TAB;
		case GLFW_KEY_PRINT_SCREEN: return WIP_PRINT_SCREEN;
		case GLFW_KEY_SCROLL_LOCK: return WIP_SCROLL_LOCK;
		case GLFW_KEY_PAUSE: return WIP_PAUSE;
		case GLFW_KEY_ENTER: return WIP_ENTER;
		case GLFW_KEY_LEFT_CONTROL: return WIP_L_CTRL;
		case GLFW_KEY_LEFT_SHIFT: return WIP_L_SHIFT;
		case GLFW_KEY_LEFT_SUPER: return WIP_L_GUI;
		case GLFW_KEY_LEFT_ALT: return WIP_L_ALT;
		case GLFW_KEY_RIGHT_CONTROL: return WIP_R_CTRL;
		case GLFW_KEY_RIGHT_SHIFT: return WIP_R_SHIFT;
		case GLFW_KEY_RIGHT_SUPER: return WIP_R_GUI;
		case GLFW_KEY_RIGHT_ALT: return WIP_R_ALT;
		case GLFW_KEY_INSERT: return WIP_INSERT;
		case GLFW_KEY_HOME: return WIP_HOME;
		case GLFW_KEY_END: return WIP_END;
		case GLFW_KEY_PAGE_UP: return WIP_PAGEUP;
		case GLFW_KEY_PAGE_DOWN: return WIP_PAGEDOWN;
		case GLFW_KEY_ESCAPE: return WIP_ESCAPE;
		case GLFW_KEY_RIGHT: return WIP_RIGHT;
		case GLFW_KEY_LEFT: return WIP_LEFT;
		case GLFW_KEY_DOWN: return WIP_DOWN;
		case GLFW_KEY_UP: return WIP_UP;
		case GLFW_KEY_SPACE: return WIP_SPACE;
		case GLFW_KEY_CAPS_LOCK: return WIP_CAPS_LOCK;
		case GLFW_KEY_MENU: return WIP_MENU;
		case GLFW_KEY_DELETE: return WIP_DELETE;
		default: break;
	}
	if(key >= GLFW_KEY_0 && key <= GLFW_KEY_9) return key;
	if(key >= GLFW_KEY_A && key <= GLFW_KEY_Z) return key + ('a' - 'A');
	if(key >= GLFW_KEY_F1 && key <= GLFW_KEY_F25) return key - GLFW_KEY_F1 + WIP_F+1;
	wip_log(WIP_WARN, "GLFW: Couldn't unify keycode: %d", key);
	return WIP_UNKNOWN;
}

void error_callback(int error, const char *message) {
	wip_log(WIP_ERROR, "GLFW: %s", message);
	return;
}

void window_close_callback(GLFWwindow *window) {
	wip_debug(WIP_INFO, "GLFW: Close requested by environment...");
	//glfwSetWindowShouldClose(window, GLFW_FALSE);
	return;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(action == GLFW_REPEAT) return;
	wip_key_t nkey = { action == GLFW_PRESS ? WIP_PRESS : WIP_RELEASE, unifyKey(key) };
	if(!wip_writeKey(nkey)) wip_log(WIP_WARN, "GLFW: Dropped input key.");
	return;
}

void wip_initWindow(void) {
	wip_debug(WIP_INFO, "%s: Initializing window...", __func__);

	if(!glfwInit())
		wip_log(WIP_FATAL, "%s: Couldn't initialize GLWF.", __func__);

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, wip_getConfInt("graphics.msaa"));
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	int width = wip_getConfInt("video.width");
	int height = wip_getConfInt("video.height");
	int fullscreen = wip_getConfBool("video.fullscreen");
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if(width == 0) {
		width = mode->width / (!fullscreen+1);
		wip_setConfInt("video.width", width);
	}
	if(height == 0) {
		height = mode->height / (!fullscreen+1);
		wip_setConfInt("video.height", height);
	}

	wip_globalWindow.handle =
		glfwCreateWindow(width, height, "WIP", fullscreen ? monitor : NULL, NULL);

	if(!wip_globalWindow.handle)
		wip_log(WIP_FATAL, "%s: Couldn't create window.", __func__);

	glfwMakeContextCurrent(wip_globalWindow.handle);
	glfwSwapInterval(wip_getConfBool("video.vsync"));

	glfwSetWindowCloseCallback(wip_globalWindow.handle, window_close_callback);
	glfwSetKeyCallback(wip_globalWindow.handle, key_callback);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
}

void wip_swapWindow(void) {
	glfwSwapBuffers(wip_globalWindow.handle);
}

void wip_pollWindow(void) {
	glfwPollEvents();
	if(glfwWindowShouldClose(wip_globalWindow.handle)) wip_globalWindow.close = 1;
}

double wip_timeWindow(void) {
	return glfwGetTime();
}

// TODO: if needed, better cleanup
void wip_termWindow(void) {
	wip_debug(WIP_INFO, "%s: Terminating window...", __func__);
	glfwTerminate();
	wip_debug(WIP_INFO, "%s: Done.", __func__);
}

