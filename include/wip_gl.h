// WIP
// Copyright (c) 2020 Jovan Lanik

// GL Functions

#pragma once

#include "external/glad/glad.h"

#ifdef NDEBUG
#define wip_glError(...) /* nothing */
#else
#define wip_glError() _wip_glError(__func__)
#endif
void _wip_glError(const char *func);
void wip_glInit(void);
GLuint wip_loadShader(const GLchar *source, GLenum type);
GLuint wip_loadProgram(GLuint vert, GLuint frag);

