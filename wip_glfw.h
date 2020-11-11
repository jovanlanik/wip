//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GLFW Functions

#pragma once

GLFWwindow *wip_initWindow(const char *name);

void error_callback(int e, const char *m);
void window_close_callback(GLFWwindow *w);
void key_callback(GLFWwindow *w, int k, int s, int a, int m);

