//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GLFW Callback Functions

#include "callback.h"

void error_callback(int error, const char *message) {
	wip_log(WIP_ERROR, "GLFW error: %s", message);
}

