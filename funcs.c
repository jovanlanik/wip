//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Functions

#include "funcs.h"

void wip_log(enum wip_logType type, const char *message, ...) {
	FILE *out = type < WIP_WARN ? stdout : stderr;
	va_list args;
	va_start(args, message);
	vfprintf(out, message, args);
	fputc('\n', out);
	va_end(args);
	if(type == WIP_FATAL) exit(1);
	return;
}

void _wip_glError(const char *func) {
	GLenum error;
	while((error = glGetError()) !=  GL_NO_ERROR) {
		wip_log(WIP_ERROR, "%s: GL error: %x", func, error);
	}
	return;
}

GLFWwindow *wip_initWindow(const char *name) {
	wip_debug(WIP_INFO, "%s: Initializing window...", __func__);
	if(!glfwInit())
		wip_log(WIP_FATAL, "Couldn't initialize GLWF.");
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow *window = glfwCreateWindow(512, 512, name, NULL, NULL);
	if(!window) {
		wip_log(WIP_FATAL, "Couldn't create window.");
	}
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwMakeContextCurrent(window);

	wip_debug(WIP_INFO, "%s: Initializing GL...", __func__);
	glewInit();
	if(!GLEW_VERSION_3_2)
		wip_log(WIP_FATAL, "OpenGL 3.3 not supported.");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, 512, 512);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return window;
}

GLuint wip_loadShader(const GLchar *source, GLenum type) {
	wip_debug(WIP_INFO, "%s: Loading shader...", __func__);
	GLuint shader = glCreateShader(type);
	if(shader == 0) {
		wip_log(WIP_ERROR, "%s: Couldn't create shader: %x", __func__, glGetError());
		return 0;
	}
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	GLint result = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		char str[2048];
		glGetShaderInfoLog(shader, sizeof(str), NULL, str);
		wip_log(WIP_ERROR, "%s: Couldn't compile shader:\n%s", __func__, str);
		glDeleteShader(shader);
		return 0;
	}
	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return shader;
}

