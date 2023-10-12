// WIP
// Copyright (c) 2020 Jovan Lanik

// GL Functions

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_gl.h"

#define wip_osmesa 1
#if WIP_WINDOW_BACKEND == wip_osmesa
#define WIP_OSMESA
#include <GL/osmesa.h>
#endif
#undef wip_osmesa

void _wip_printGlErrors(const char *func) {
	GLenum error;
	while((error = glGetError()) !=  GL_NO_ERROR) {
		wip_log(WIP_ERROR, "%s: GL error: %x", func, error);
	}
	return;
}

void wip_initGl(void) {
	wip_debug(WIP_INFO, "%s: Initializing OpenGL...", __func__);
#ifndef WIP_OSMESA
	if(!gladLoadGL())
#else
	if(!gladLoadGLLoader((GLADloadproc)OSMesaGetProcAddress))
#endif
		wip_log(WIP_FATAL, "%s: Couldn't initialize OpenGL.", __func__);
	if(!GLAD_GL_VERSION_3_3)
		wip_log(WIP_FATAL, "%s: OpenGL 3.3 not supported.", __func__);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, wip_getConfInt("video.width"), wip_getConfInt("video.height"));

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
}

#ifdef NDEBUG
#define DEBUG_LOG_BUFFER_LEN 1024
#else
#define DEBUG_LOG_BUFFER_LEN 8192
#endif

GLuint wip_loadShader(const GLchar *source, GLenum type) {
	wip_debug(WIP_INFO, "%s: Loading shader...", __func__);
	GLuint shader = glCreateShader(type);
	if(shader == 0) {
		wip_log(WIP_ERROR, "%s: Couldn't create shader: %x", __func__, glGetError());
		return 0;
	}
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE) {
		char str[DEBUG_LOG_BUFFER_LEN];
		glGetShaderInfoLog(shader, sizeof(str), NULL, str);
		wip_log(WIP_ERROR, "%s: Couldn't compile shader:\n%s", __func__, str);
		glDeleteShader(shader);
		return 0;
	}
	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return shader;
}

GLuint wip_loadProgram(GLuint vert, GLuint frag) {
	GLuint program = glCreateProgram();
	if(program == 0) {
		wip_log(WIP_ERROR, "%s: Couldn't create program: %x", __func__, glGetError());
		return 0;
	}
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if(result == GL_FALSE) {
		char str[DEBUG_LOG_BUFFER_LEN];
		glGetProgramInfoLog(program, sizeof(str), NULL, str);
		wip_log(WIP_ERROR, "%s: Couldn't link program:\n%s", __func__, str);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

