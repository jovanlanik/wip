// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Rendering Functions

#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <GL/glew.h>

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_img.h"
#include "wip_obj.h"
#include "wip_mdl.h"
#include "wip_game.h"
#include "wip_window.h"
#include "wip_gl.h"
#include "wip_math.h"
#include "lib/linmath.h"

#include "d_format.h"

#include "baked/shaders.h"

extern wip_window_t wip_globalWindow;
//extern pthread_mutex_t wip_globalWindow_m;

extern wip_scene_t wip_globalScene;

unsigned int lightLocation;
unsigned int mpvLocation;
unsigned int transformLocation;
unsigned int normalTransformLocation;
unsigned int materialLocation;

GLuint program;

float m[3] = { -1.5, 2.0, 0.4 };

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

void drawModel(wip_obj_t *object, wip_glmdl_t *model, wip_globj_t pv, wip_obj_t *light) {
	// TODO: fix material;
	static int init = 0;
	static GLuint texture;
	if(!init) {
		init = 1;
		texture = wip_openTexture("d_wall");
	}

	wip_globj_t mpv;
	wip_globj_t transform;
	wip_loadObject(&transform, object);
	wip_globj_t normalTransform;
	{
		wip_globj_t go;
		mat4x4_invert(go.m, transform.m);
		mat4x4_transpose(normalTransform.m, go.m);
	}
	mat4x4_mul(mpv.m, pv.m, transform.m);
	wip_loadObjectF(WIP_POS, &transform, object);

	glBindVertexArray(model->vertex_a);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUseProgram(program);
	if(light) glUniform3fv(lightLocation, 1, light->position);
	glUniformMatrix4fv(mpvLocation, 1, GL_FALSE, mpv.f);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transform.f);
	glUniformMatrix4fv(normalTransformLocation, 1, GL_FALSE, normalTransform.f);
	glUniform3fv(materialLocation, 1, m);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, model->element_c, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	return;
}

void drawRoom(room_t *room, wip_globj_t pv) {
	for(int i = 0; i < room->width; ++i) {
		for(int n = 0; n < room->height; ++n) {
			wip_obj_t obj;
			wip_makeObject(&obj);
			obj.x = 2*i - 10;
			obj.y = 2*n - 10;
			if(*room->deco[i][n] == DECO_MODEL)
				drawModel(&obj, ((struct deco_model*)room->deco[i][n])->model, pv, NULL);
		}
	}
	return;
}

void *wip_renderThread(void *arg) {
	wip_setWindow();
	wip_glInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint vertShader = wip_loadShader((char *)d_texture_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char *)d_texture_frag, GL_FRAGMENT_SHADER);
	program = wip_loadProgram(vertShader, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	//wip_glmdl_t *wall_model = wip_openModel("d_wall");
	//wip_glmdl_t *floor_model = wip_openModel("d_floor");
	wip_glmdl_t *sword_model = wip_openModel("d_sword");

	lightLocation = glGetUniformLocation(program, "light");
	mpvLocation = glGetUniformLocation(program, "mpv");
	transformLocation = glGetUniformLocation(program, "transform");
	normalTransformLocation = glGetUniformLocation(program, "normalTransform");
	materialLocation = glGetUniformLocation(program, "material");

	while(!wip_globalScene.length);

	wip_obj_t *center = wip_globalScene.object[0];
	wip_obj_t *camera = wip_globalScene.object[1];
	//wip_obj_t light;
	//wip_makeObject(&light);
	//light.z = -1.0;

	room_t *fake = testRoom();
	dungeon_t d;
	readDungeon(&d, "./dungeon.d/example.df");

	vec3 axis = {0.0f, 0.0f, 1.0f};
	wip_globj_t projection, ui;
	mat4x4_identity(ui.m);
	float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
	mat4x4_perspective(projection.m, TO_RAD(wip_getConfFloat("game.fov")), ratio, 0.1, 1000);

	int fpsMax = wip_getConfInt("video.fpsMax");
	double startTime, lastTime = wip_timeWindow();

	while(!wip_globalWindow.close) {
		lastTime = startTime;
		startTime = wip_timeWindow();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		wip_globj_t pv, view;

		mat4x4_look_at(view.m, camera->position, center->position, axis);
		mat4x4_mul(pv.m, projection.m, view.m);

		// Render here...
		drawRoom(fake, pv);
		// Viewmodel
		glClear(GL_DEPTH_BUFFER_BIT);
		drawModel(camera, sword_model, pv, NULL);
		// HUD
		glClear(GL_DEPTH_BUFFER_BIT);
		drawStr(10, 10, 3.0f, "Copyright (c) 2021 Jovan Lanik");
		drawFormatStr(2*wip_getConfInt("video.width")-130, 10, 2.0f, "%4.1f", (startTime - lastTime) * 1000.0f);
		
		//wip_glError();

		do wip_swapWindow();
		while(wip_timeWindow() - startTime < 1.0/fpsMax && fpsMax && !wip_globalWindow.close);
	}

	pthread_exit(NULL);
}

