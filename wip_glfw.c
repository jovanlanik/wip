//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GLFW Functions

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
	GLFWwindow *window = glfwCreateWindow(900, 600, name, NULL, NULL);
	if(!window) {
		wip_log(WIP_FATAL, "Couldn't create window.");
	}
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwMakeContextCurrent(window);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return window;
}

