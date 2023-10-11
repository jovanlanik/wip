// Flipdot demo
// Copyright (c) 2023 Jovan Lanik

// Main game

#define _POSIX_C_SOURCE 200808L

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_img.h"
#include "wip_obj.h"
#include "wip_game.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_gl.h"
#include "wip_mdl.h"
#include "external/linmath.h"
#include "external/wfdd.h"

extern const char _binary_flipdot_conf_start[];
extern const char _binary_flipdot_vert_start[];
extern const char _binary_flipdot_frag_start[];

const char *wip_defaultConf = _binary_flipdot_conf_start;

extern int wip_globalKeyLock;
extern wip_window_t wip_globalWindow;

vec3 axis = {0.0f, 0.0f, 1.0f};
GLuint program;

#ifdef GL_TRUE
#define UINT GLuint
#else
#define UINT uint32_t
#endif
#define BUFF2(type) { type data_b; type element_b; }

static void drawScreen(void) {
	const float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	const unsigned int indices[] = {
		0, 1, 2, 3,
	};

	static int init = 0;
	static GLuint texture = 0;
	static union WIP_NAMED_VEC_T(2, UINT, BUFF2, buffers, ) render_object;
	static GLuint vertex_a = 0;
	static GLuint program = 0;
	static GLuint timeLocation = 0;
	if(init == 0) {
		init = 1;

		glGenVertexArrays(1, &vertex_a);
		glBindVertexArray(vertex_a);

		glGenBuffers(2, render_object.buffers);
		glBindBuffer(GL_ARRAY_BUFFER, render_object.data_b);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_object.element_b);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		texture = wip_openTexture("takt");
		GLuint vert = wip_loadShader((char *)_binary_flipdot_vert_start, GL_VERTEX_SHADER);
		GLuint frag = wip_loadShader((char *)_binary_flipdot_frag_start, GL_FRAGMENT_SHADER);
		program = wip_loadProgram(vert, frag);

		timeLocation = glGetUniformLocation(program, "time");
	}

	glUseProgram(program);
	glBindVertexArray(vertex_a);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUniform1f(timeLocation, wip_timeWindow());
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	return;
}

#undef UINT
#undef BUFF2

#define wip_osmesa 1
#if WIP_WINDOW_BACKEND == wip_osmesa
	#define WIP_OSMESA
#endif
#undef wip_osmesa

void wip_gameLoop(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	int cout = wip_getConfBool("game.console_output");
	const char *addr = wip_getConfStr("game.display_address");
	int port = wip_getConfInt("game.display_port");

	wfdd_display_t *display = wfdd_initDisplay(addr, port);

	int w = wip_getConfInt("video.width");
	int h = wip_getConfInt("video.height");
	int d = w * h;
	int f = d * sizeof(uint8_t);
	int s = d / 8;
	uint8_t *fbuffer = wip_alloc(f);
	uint8_t *sbuffer = wip_alloc(s);

	double skip = 1.0 / wip_getConfInt("game.fps_max");
	double next = wip_timeWindow();

	while(!wip_globalWindow.close) {
#ifdef WIP_OSMESA
		wip_key_t key;
		while((key = wip_readKey()).action) {
			if(key.key == WIP_LOCKED) break;
			wip_writeMotion(key);
			if(wip_readMotion(QUIT)) wip_globalWindow.close = 1;
		}
#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawScreen();
		glReadPixels(0, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, fbuffer);

		memset(sbuffer, 0, s);
		for(int i = 0; i < d; ++i) {
			int xi = (i/w) + (i%w) * h;
			int ii = xi % 8;
			int yi = xi / 8;
			int v = fbuffer[i] > 20;
			if(cout && !(i%w)) printf("\n");
			if(cout) printf("%c", v ? '#' : '-');
			sbuffer[yi] = (sbuffer[yi] & ~(1 << ii)) | (v << ii);
		}
		if(cout) printf("\n\n");

		wfdd_sendBuffer(display, s, sbuffer);

#ifdef WIP_OSMESA
		wip_pollWindow();
		wip_swapWindow();
#endif

		next += skip;
		wip_sleep(next - wip_timeWindow());
	}

	wfdd_closeDisplay(display);
	wip_free(sbuffer);
	wip_free(fbuffer);

	return;
}

