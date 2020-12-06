//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// GL Functions

#include <string.h>
#include <math.h>
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

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

extern wip_scene_t wip_globalScene;
extern pthread_mutex_t wip_globalScene_m;

void *wip_renderThread(void *arg) {
	wip_setWindow();
	wip_glInit();

	glClearColor(0.3f, 0.6f, 0.8f, 1.0f);

	GLuint vertShader = wip_loadShader((char *)glsl_main_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char *)glsl_main_frag, GL_FRAGMENT_SHADER);
	GLuint vert2Shader = wip_loadShader((GLchar *)glsl_invertedHull_vert, GL_VERTEX_SHADER);
	GLuint frag2Shader = wip_loadShader((GLchar *)glsl_outline_frag, GL_FRAGMENT_SHADER);
	GLuint program = wip_loadProgram(vertShader, fragShader);
	GLuint program2 = wip_loadProgram(vert2Shader, frag2Shader);

	glDeleteShader(vertShader);
	glDeleteShader(vert2Shader);
	glDeleteShader(fragShader);
	glDeleteShader(frag2Shader);

	wip_ply_t ply;
	wip_mdl_t mdl;
	wip_glmdl_t glmdl, glmdl_outline;
	mdl.vertex_c = wip_allocType(int);
	mdl.index_c = wip_allocType(int);

	wip_readModel(&ply, "mdl/wip_model.ply");

	wip_prepModel(&mdl, &ply);
	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);

	wip_readModel(&ply, "mdl/wip_model_outline.ply");

	wip_loadModel(&glmdl, &mdl);
	wip_free(mdl.model);
	wip_free(mdl.index);

	wip_prepModel(&mdl, &ply);
	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);

	wip_loadModel(&glmdl_outline, &mdl);
	wip_free(mdl.model);
	wip_free(mdl.index);

	wip_free(mdl.vertex_c);
	wip_free(mdl.index_c);

	unsigned int lightLocation = glGetUniformLocation(program, "light");
	unsigned int mpvLocation = glGetUniformLocation(program, "mpv");
	unsigned int transformLocation = glGetUniformLocation(program, "transform");
	unsigned int normalTransformLocation = glGetUniformLocation(program, "normalTransform");
	unsigned int materialLocation = glGetUniformLocation(program, "material");
	unsigned int mpvLocation2 = glGetUniformLocation(program2, "mpv");
	unsigned int outlineThicknessLocation2 = glGetUniformLocation(program2, "outlineThickness");
	unsigned int outlineColorLocation2 = glGetUniformLocation(program2, "outlineColor");

	float m[3] = { 0.1, 0.1, 0.4 };
	
	while(!wip_globalScene.length);

	wip_obj_t *light = wip_globalScene.object[0];
	wip_obj_t *eye = wip_globalScene.object[1];

	vec3 axis = {0.0f, 0.0f, 1.0f};
	wip_globj_t projection;
	float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
	mat4x4_perspective(projection.m, RAD(wip_getConfFloat("game.fov")), ratio, 0.1, 100);

	int fps = 0;
	double lt = 0;
	double ct;

	while(!wip_globalWindow.close) {
		ct = wip_timeWindow();
		fps++;
		if(ct - lt >= 1.0) {
			lt = ct;
			//wip_log(WIP_INFO, "Framerate: %d\nFrametime: %f\n", fps, 1000.0/fps);
			fps = 0;
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		wip_obj_t center;
		wip_globj_t pv, view;

		center = *eye;
		center.x += sin(eye->r.y);
		center.y += cos(eye->r.y);
		
		mat4x4_look_at(view.m, eye->position, center.position, axis);
		mat4x4_mul(pv.m, projection.m, view.m);
		
		for(int i = 2; i < wip_globalScene.length; ++i) {
			wip_globj_t mpv;
			wip_globj_t transform;
			wip_loadObject(&transform, wip_globalScene.object[i]);
			wip_globj_t normalTransform;
			{
				wip_globj_t go;
				mat4x4_invert(go.m, transform.m);
				mat4x4_transpose(normalTransform.m, go.m);
			}
			mat4x4_mul(mpv.m, pv.m, transform.m);
			wip_loadObjectF(WIP_POS, &transform, wip_globalScene.object[i]);

			glBindVertexArray(glmdl.vertex_a);

			glUseProgram(program);
			glUniform3fv(lightLocation, 1, light->position);
			glUniformMatrix4fv(mpvLocation, 1, GL_FALSE, mpv.f);
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transform.f);
			glUniformMatrix4fv(normalTransformLocation, 1, GL_FALSE, normalTransform.f);
			glUniform3fv(materialLocation, 1, m);
			glDrawElements(GL_TRIANGLES, glmdl.element_c, GL_UNSIGNED_INT, 0);

			glBindVertexArray(glmdl_outline.vertex_a);

			glUseProgram(program2);
			glUniformMatrix4fv(mpvLocation2, 1, GL_FALSE, mpv.f);
			glUniform1f(outlineThicknessLocation2, 0.05);
			glUniform3fv(outlineColorLocation2, 1, (float []){0.1, 0, 0});
			glCullFace(GL_FRONT);
			glDrawElements(GL_TRIANGLES, glmdl_outline.element_c, GL_UNSIGNED_INT, 0);
			glCullFace(GL_BACK);
			//wip_glError();
	
			glBindVertexArray(0);

		}

		wip_swapWindow();
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

	glViewport(0, 0, wip_getConfInt("video.width"), wip_getConfInt("video.height"));

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
	GLint result;
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
		wip_log(WIP_ERROR, "%s: Couldn't link program.", __func__);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

