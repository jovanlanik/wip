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
}

void window_close_callback(GLFWwindow* window) {
	wip_log(WIP_INFO, "Window tried to close.");
	glfwSetWindowShouldClose(window, GLFW_FALSE);
}

