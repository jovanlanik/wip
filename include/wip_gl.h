// WIP
// Copyright (c) 2020 Jovan Lanik

// GL Functions

#pragma once

#include "external/glad/glad.h"

#ifdef NDEBUG
#define wip_printGlErrors(...) /* nothing */
#else
#define wip_printGlErrors() _wip_printGlErrors(__func__)
#endif
void _wip_printGlErrors(const char *func);
void wip_initGl(void);
GLuint wip_loadShader(const GLchar *source, GLenum type);
GLuint wip_loadProgram(GLuint vert, GLuint frag);

