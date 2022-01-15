// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Text drawing

#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "wip_fn.h"
#include "wip_gl.h"
#include "wip_mdl.h"
#include "wip_img.h"
#include "wip_conf.h"

extern const char text_vert[];
extern const char text_frag[];

void drawChar(char c, unsigned int x, unsigned int y, unsigned int ox, unsigned int oy, float scale) {
	// TODO: you're leaking memory here by allocating the model and losing the pointer...
	static wip_glmdl_t *model = NULL;
	static GLuint texture;
	static unsigned int cLocation;
	static unsigned int screenLocation;
	static unsigned int positionLocation;
	static unsigned int offsetLocation;
	static unsigned int scaleLocation;
	static GLuint textProgram;
	if(!model) {
		model = wip_openModel("char");
		texture = wip_openTexture("font");
		GLuint textVert = wip_loadShader(text_vert, GL_VERTEX_SHADER);
		GLuint textFrag = wip_loadShader(text_frag, GL_FRAGMENT_SHADER);
		textProgram = wip_loadProgram(textVert, textFrag);
		glDeleteShader(textVert);
		glDeleteShader(textFrag);
		cLocation = glGetUniformLocation(textProgram, "c");
		screenLocation = glGetUniformLocation(textProgram, "screen");
		positionLocation = glGetUniformLocation(textProgram, "position");
		offsetLocation = glGetUniformLocation(textProgram, "offset");
		scaleLocation = glGetUniformLocation(textProgram, "scale");
	}

	glBindVertexArray(model->vertex_a);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned int s[] = { wip_getConfInt("video.width"), wip_getConfInt("video.height") };
	unsigned int p[] = { x, y };
	unsigned int o[] = { ox, oy };

	glUseProgram(textProgram);
	glUniform1ui(cLocation, c);
	glUniform2uiv(screenLocation, 1, s);
	glUniform2uiv(positionLocation, 1, p);
	glUniform2uiv(offsetLocation, 1, o);
	glUniform1f(scaleLocation, scale);
	glDrawElements(GL_TRIANGLES, model->element_c, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	return;
}

void drawStr(unsigned int x, unsigned int y, float scale, char *str) {
	int i = 0, j = 0, k = 0;
	while(str[i] != '\0') {
		switch(str[i]) {
			case '\n': i++; j = 0; k++; continue;
			//TODO: handle tab
			default: drawChar(isprint(str[i]) ? str[i] : 127, x, y, j, k, scale);
		}
		++i; ++j;
	}
}

void drawFormatStr(unsigned int x, unsigned int y, float scale, char *format, ...) {
	size_t len = 2*strlen(format);
	char *str = wip_alloc(len);
	va_list args;
	va_start(args, format);
	int n = vsnprintf(str, len, format, args);
	if(n > len-1) {
		va_end(args);
		va_start(args, format);
		wip_realloc(str, n+1, NULL);
		vsnprintf(str, n+1, format, args);
	}
	va_end(args);
	drawStr(x, y, scale, str);
	wip_free(str);
}


