//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GL Functions

#pragma once

void _wip_glError(const char *func);
#define wip_glError(void) _wip_glError(__func__)
void wip_glInit(void);
GLuint wip_loadShader(const GLchar *source, GLenum type);
GLuint wip_mkProgram(GLuint vert, GLuint frag);

