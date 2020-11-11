//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GLFW Functions

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "wip_fn.h"
#include "wip_input.h"
#include "wip_glfw.h"

GLFWwindow *wip_initWindow(const char *name) {
	wip_debug(WIP_INFO, "%s: Initializing window...", __func__);

	if(!glfwInit())
		wip_log(WIP_FATAL, "Couldn't initialize GLWF.");
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow *window = glfwCreateWindow(1680, 1050, name, NULL, NULL);
	if(!window) {
		wip_log(WIP_FATAL, "Couldn't create window.");
	}
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return window;
}

void error_callback(int error, const char *message) {
	wip_log(WIP_ERROR, "GLFW error: %s", message);
	return;
}

void window_close_callback(GLFWwindow *window) {
	wip_debug(WIP_INFO, "Close requested, exiting...");
	//glfwSetWindowShouldClose(window, GLFW_FALSE);
	return;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	// TODO: clean up
	if(action == GLFW_REPEAT) return;
	wip_key_t nkey = { action ? 1 : 0, key };
	int ret = wip_keyWrite(nkey);
	wip_debug(
		ret ? WIP_INFO: WIP_ERROR,
		"Input %s: %s %s",
		ret ? "sent" : "dropped",
	 	action ? "Pressed" : "Released",
		glfwGetKeyName(key, 0)
		);
	return;
}

