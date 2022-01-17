// WIP
// Copyright (c) 2020 Jovan Lanik

// Demo game

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_obj.h"
#include "wip_mdl.h"
#include "wip_input.h"
#include "wip_game.h"
#include "wip_window.h"
#include "wip_gl.h"
#include "wip_math.h"
#include "external/linmath.h"
#include "baked/shaders.h"
#include "baked/demo_config.h"

extern wip_window_t wip_globalWindow;
extern char wip_globalKeyName[][64];

const char *wip_defaultConf = demo_conf;

wip_obj_t *getPointOnCircle(wip_obj_t *result, wip_obj_t *center, float angle, float radius) {
	*result = *center;
	result->x += radius * sin(angle);
	result->y += radius * cos(angle);
	return result;
}

void wip_gameLoop(void) {
	wip_obj_t object,light, eye, center;

	wip_makeObject(&eye);
	eye.y = -3;

	wip_makeObject(&center);
	getPointOnCircle(&center, &eye, eye.r.y, 1);

	wip_makeObject(&light);
	light.x = 2;
	light.y = 0;
	light.z = 1;

	wip_makeObject(&object);

	glClearColor(0.3f, 0.6f, 0.8f, 1.0f);

	GLuint vertShader = wip_loadShader((char *)main_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char *)main_frag, GL_FRAGMENT_SHADER);
	GLuint program = wip_loadProgram(vertShader, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	wip_ply_t ply;
	wip_mdl_t mdl;
	wip_glmdl_t glmdl;

	wip_readModel(&ply, "./res/mdl/demo.ply");

	wip_prepModel(&mdl, &ply);
	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);

	wip_loadModel(&glmdl, &mdl);
	wip_free(mdl.model);
	wip_free(mdl.index);

	unsigned int lightLocation = glGetUniformLocation(program, "light");
	unsigned int mpvLocation = glGetUniformLocation(program, "mpv");
	unsigned int transformLocation = glGetUniformLocation(program, "transform");
	unsigned int normalTransformLocation = glGetUniformLocation(program, "normalTransform");
	unsigned int materialLocation = glGetUniformLocation(program, "material");

	float m[3] = { 0.1, 0.1, 0.4 };

	vec3 axis = {0.0f, 0.0f, 1.0f};
	wip_globj_t projection;
	float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
	mat4x4_perspective(projection.m, TO_RAD(wip_getConfFloat("game.fov")), ratio, 0.1, 100);

	int fpsMax = wip_getConfInt("video.fpsMax");
	double startTime;

	while(!wip_globalWindow.close) {
		startTime = wip_timeWindow();

		wip_key_t key;
		while((key = wip_readKey()).action) {
			if(key.key == WIP_ESC || key.key == 'q')
				wip_globalWindow.close = 1;
			wip_writeMotion(key);
		}
		if(wip_readMotion(MOVE_FORWARD)) {
			eye.y -= eye.y - center.y;
			eye.x -= eye.x - center.x;
		}
		if(wip_readMotion(MOVE_BACK)) {
			eye.y += eye.y - center.y;
			eye.x += eye.x - center.x;
		}
		getPointOnCircle(&center, &eye, eye.r.y, 0.025);
		if(wip_readMotion(STRAFE_LEFT)) eye.r.y -= 0.02;
		if(wip_readMotion(STRAFE_RIGHT)) eye.r.y += 0.02;

		object.z = 0.5*sin(startTime);
		quat rot, cop;
		quat_dup(cop, object.rotation);
		quat_rotate(rot, TO_RAD(0.15f), (float[]){0, 1, 0});
		quat_mul(object.rotation, cop, rot);
		quat_norm(object.rotation, object.rotation);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		wip_globj_t pv, view;

		mat4x4_look_at(view.m, eye.position, center.position, axis);
		mat4x4_mul(pv.m, projection.m, view.m);

		wip_globj_t mpv;
		wip_globj_t transform;
		wip_loadObject(&transform, &object);
		wip_globj_t normalTransform;
		{
			wip_globj_t go;
			mat4x4_invert(go.m, transform.m);
			mat4x4_transpose(normalTransform.m, go.m);
		}
		mat4x4_mul(mpv.m, pv.m, transform.m);
		wip_loadObjectF(WIP_POS, &transform, &object);

		glBindVertexArray(glmdl.vertex_a);

		glUseProgram(program);
		glUniform3fv(lightLocation, 1, light.position);
		glUniformMatrix4fv(mpvLocation, 1, GL_FALSE, mpv.f);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transform.f);
		glUniformMatrix4fv(normalTransformLocation, 1, GL_FALSE, normalTransform.f);
		glUniform3fv(materialLocation, 1, m);
		glDrawElements(GL_TRIANGLES, glmdl.element_c, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		wip_pollWindow();
		do wip_swapWindow();
		while(wip_timeWindow() - startTime < 1.0/fpsMax && fpsMax && !wip_globalWindow.close);
	}

}

