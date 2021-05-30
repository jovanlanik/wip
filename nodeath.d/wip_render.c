//
// WIP
// Copyright (c) 2021 Jovan Lanik
//

// Rendering Functions

#include <string.h>
#include <pthread.h>
#include <GL/glew.h>

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_obj.h"
#include "wip_mdl.h"
#include "wip_game.h"
#include "wip_window.h"
#include "wip_gl.h"
#include "wip_math.h"
#include "lib/linmath.h"

#include "baked/shaders.h"

int wip_globalFramesPerSecond;

extern wip_window_t wip_globalWindow;
//extern pthread_mutex_t wip_globalWindow_m;

extern wip_scene_t wip_globalScene;
//extern pthread_mutex_t wip_globalScene_m;

wip_glmdl_t *openModel(char *name) {
	wip_ply_t ply;
	wip_mdl_t mdl;
	wip_glmdl_t *glmdl = wip_alloc(sizeof(wip_glmdl_t));

	mdl.vertex_c = wip_alloc(sizeof(int));
	mdl.index_c = wip_alloc(sizeof(int));

	const char *prefix = "./mdl/";
	const char *suffix = ".ply";
	char *filename = wip_alloc(strlen(prefix)+strlen(name)+strlen(suffix)+1);
	sprintf(filename, "%s%s%s", prefix, name, suffix);

	wip_readModel(&ply, filename);

	wip_prepModel(&mdl, &ply);
	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);

	wip_loadModel(glmdl, &mdl);
	wip_free(mdl.model);
	wip_free(mdl.index);

	wip_free(mdl.vertex_c);
	wip_free(mdl.index_c);

	return glmdl;
}

unsigned int lightLocation;
unsigned int mpvLocation;
unsigned int transformLocation;
unsigned int normalTransformLocation;
unsigned int materialLocation;

GLuint program;

void drawModel(wip_obj_t *object, wip_glmdl_t *model, wip_globj_t pv, wip_obj_t *light) {
	const float m[3] = { 0.1, 0.1, 0.4 };

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

	glUseProgram(program);
	glUniform3fv(lightLocation, 1, light->position);
	glUniformMatrix4fv(mpvLocation, 1, GL_FALSE, mpv.f);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transform.f);
	glUniformMatrix4fv(normalTransformLocation, 1, GL_FALSE, normalTransform.f);
	glUniform3fv(materialLocation, 1, m);
	glDrawElements(GL_TRIANGLES, model->element_c, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	return;
}

void *wip_renderThread(void *arg) {
	wip_setWindow();
	wip_glInit();

	glClearColor(0.3f, 0.6f, 0.8f, 1.0f);

	GLuint vertShader = wip_loadShader((char *)main_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char *)main_frag, GL_FRAGMENT_SHADER);
	program = wip_loadProgram(vertShader, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	wip_glmdl_t *playerModel = openModel("player");
	wip_glmdl_t *enemyModel = openModel("enemy");

	lightLocation = glGetUniformLocation(program, "light");
	mpvLocation = glGetUniformLocation(program, "mpv");
	transformLocation = glGetUniformLocation(program, "transform");
	normalTransformLocation = glGetUniformLocation(program, "normalTransform");
	materialLocation = glGetUniformLocation(program, "material");

	while(!wip_globalScene.length);

	wip_obj_t *light = wip_globalScene.object[0];
	wip_obj_t *eye = wip_globalScene.object[1];
	wip_obj_t *center = wip_globalScene.object[2];
	wip_obj_t *player = wip_globalScene.object[3];

	vec3 axis = {0.0f, 1.0f, 0.0f};
	wip_globj_t projection;
	float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
	mat4x4_perspective(projection.m, RAD(wip_getConfFloat("game.fov")), ratio, 0.1, 1000);

	wip_globalFramesPerSecond = 0;
	int fpsMax = wip_getConfInt("video.fpsMax");
	double startTime, lastTime = wip_timeWindow();

	while(!wip_globalWindow.close) {
		startTime = wip_timeWindow();

		wip_globalFramesPerSecond++;
		if(startTime - lastTime >= 1.0) {
			lastTime = startTime;
			//wip_log(WIP_INFO, "Framerate: %d\nFrametime: %f\n",
			//	wip_globalFramesPerSecond, 1000.0/wip_globalFramesPerSecond);
			wip_globalFramesPerSecond = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		wip_globj_t pv, view;

		mat4x4_look_at(view.m, eye->position, center->position, axis);
		mat4x4_mul(pv.m, projection.m, view.m);

		drawModel(player, playerModel, pv, light);

		for(int i = 4; i < wip_globalScene.length; ++i) {
			drawModel(wip_globalScene.object[i], enemyModel, pv, light);
		}

		do wip_swapWindow();
		while(wip_timeWindow() - startTime < 1.0/fpsMax && fpsMax && !wip_globalWindow.close);
	}

	pthread_exit(NULL);
}

