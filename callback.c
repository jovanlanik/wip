//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GLFW Callback Functions

#include <GLFW/glfw3.h>

#include "wip_fn.h"
#include "callback.h"

void error_callback(int error, const char *message) {
	wip_log(WIP_ERROR, "GLFW error: %s", message);
	return;
}

void window_close_callback(GLFWwindow* window) {
	wip_debug(WIP_INFO, "Close requested, exiting...");
	//glfwSetWindowShouldClose(window, GLFW_FALSE);
	return;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_REPEAT) return;
	wip_debug(WIP_INFO, "Input received: %s %s", action ? "Pressed" : "Released", glfwGetKeyName(key, 0));
	return;
}

