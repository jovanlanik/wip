//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GL Functions

#include <pthread.h>
#include <GL/glew.h>

#include "wip_fn.h"
#include "wip_window.h"
#include "wip_gl.h"

void *wip_renderThread(void *arg) {
	wip_setWindow(&wip_globalWindow);
	wip_glInit();

	glClearColor(0.3f, 0.6f, 0.8f, 1.0f);

	while(!wip_globalWindow.close) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		wip_swapWindow(&wip_globalWindow);
	}

	pthread_exit(NULL);
}

void _wip_glError(const char *func) {
	GLenum error;
	while((error = glGetError()) !=  GL_NO_ERROR) {
		wip_log(WIP_ERROR, "%s: GL error: %x", func, error);
	}
	return;
}

void wip_glInit(void) {
	wip_debug(WIP_INFO, "%s: Initializing GL...", __func__);
	glewInit();
	if(!GLEW_VERSION_3_2)
		wip_log(WIP_FATAL, "%s: OpenGL 3.3 not supported.", __func__);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, 1680, 1050);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
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

GLuint wip_loadProgram(GLuint vert, GLuint frag) {
	// TODO: add error handling below
	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	return program;
}

