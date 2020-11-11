//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Main Source

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <pthread.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "wip_fn.h"
#include "wip_gl.h"
#include "wip_obj.h"
#include "wip_mdl.h"
#include "wip_input.h"
#include "wip_glfw.h"
#include "wip_math.h"
#include "lib/linmath.h"

#include "shaders.h"

int main(int argc, char *argv[]) {
	wip_debug(WIP_INFO, "WIP built %s %s", __DATE__, __TIME__);
	GLFWwindow *window = wip_initWindow(argv[0]);
	wip_glInit();


	GLuint vertShader = wip_loadShader((char*)glsl_main_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char*)glsl_main_frag, GL_FRAGMENT_SHADER);
	GLuint vert2Shader = wip_loadShader((GLchar*)glsl_invertedHull_vert, GL_VERTEX_SHADER);
	GLuint frag2Shader = wip_loadShader((GLchar*)glsl_outline_frag, GL_FRAGMENT_SHADER);

	GLuint program = wip_loadProgram(vertShader, fragShader);
	GLuint program2 = wip_loadProgram(vert2Shader, frag2Shader);

	glDeleteShader(vertShader);
	glDeleteShader(vert2Shader);
	glDeleteShader(fragShader);
	glDeleteShader(frag2Shader);
	

	wip_ply_t ply;
	wip_mdl_t mdl;
	wip_glmdl_t glmdl[2];

	mdl.vertex_c = wip_allocType(int);
	mdl.index_c = wip_allocType(int);

	wip_readModel(&ply, "mdl/wip_model.ply");
	wip_prepModel(&mdl, &ply);

	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);

	wip_readModel(&ply, "mdl/wip_model2.ply");
	wip_loadModel(&glmdl[0], &mdl);

	wip_free(mdl.model);
	wip_free(mdl.index);
	wip_prepModel(&mdl, &ply);

	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);
	wip_loadModel(&glmdl[1], &mdl);
	
	wip_free(mdl.vertex_c);
	wip_free(mdl.index_c);

	wip_glmdl_t *currentGlmdl = &glmdl[0];

	wip_obj_t *rocket = wip_allocType(wip_obj_t);
	wip_makeObject(rocket);
	wip_globj_t *transform = wip_allocType(wip_globj_t);


	unsigned int lightLocation = glGetUniformLocation(program, "light");
	unsigned int mpvLocation = glGetUniformLocation(program, "mpv");
	unsigned int transformLocation = glGetUniformLocation(program, "transform");
	unsigned int normalTransformLocation = glGetUniformLocation(program, "normalTransform");
	unsigned int materialLocation = glGetUniformLocation(program, "material");

	unsigned int mpvLocation2 = glGetUniformLocation(program2, "mpv");
	unsigned int outlineThicknessLocation2 = glGetUniformLocation(program2, "outlineThickness");
	unsigned int outlineColorLocation2 = glGetUniformLocation(program2, "outlineColor");

	float step = 0.1;
	float m[3] = { 0.1, 0.1, 0.4 };

	wip_obj_t light, eye, center; 

	wip_makeObject(&center);

	wip_makeObject(&eye);
	eye.y = -3;

	wip_makeObject(&light);
	light.x = -2;
	light.y = -3;
	light.z = 2;
	//light.x = 0;
	//light.y = 0;
	//light.z = 0;
	
	vec3 axis = {0.0f, 0.0f, 1.0f};
	mat4x4 view;

	mat4x4 projection;
	mat4x4_perspective(projection, RAD(70), 16.0f/10.0f, 0.1, 100);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//#define KEY(x) if(glfwGetKey(window, x) == GLFW_PRESS)
		#define KEY(x) if(k.key == x)
		wip_key_t k;
		while((k = wip_keyRead()).key != -1) {
		wip_debug(WIP_INFO, "Input read: %s %s", k.action ? "Pressed" : "Released", glfwGetKeyName(k.key, 0));
		if(k.action == 0) break;

		KEY(GLFW_KEY_W) eye.y += step;
		KEY(GLFW_KEY_S) eye.y -= step;
		KEY(GLFW_KEY_D) eye.x += step;
		KEY(GLFW_KEY_A) eye.x -= step;
		KEY(GLFW_KEY_SPACE) eye.z += step;
		KEY(GLFW_KEY_X) eye.z -= step;

		KEY(GLFW_KEY_C) m[0] += 0.01;
		KEY(GLFW_KEY_V) m[0] -= 0.01;

		KEY(GLFW_KEY_E) currentGlmdl = &glmdl[0];
		KEY(GLFW_KEY_R) currentGlmdl = &glmdl[1];
		KEY(GLFW_KEY_T) currentGlmdl = &glmdl[2];


		KEY(GLFW_KEY_Q) glfwSetWindowShouldClose(window, GLFW_TRUE);

		}
		#undef KEY

		center.x = eye.x;
		center.y = eye.y + 1;
		center.z = eye.z;
		mat4x4_look_at(view, eye.position, center.position, axis);

		//light.x = sin(glfwGetTime());

		rocket->z = 0.5*sin(glfwGetTime());
		rocket->r.y = 90;
		rocket->r.x = 25*glfwGetTime();
		rocket->s.x = 1.2;
		rocket->s.y = 1.2;
		rocket->s.z = 1.2;
		wip_loadObject(transform, rocket);

		//m[0] = M_PI*sin(glfwGetTime());
		//m[1] = 1.5*M_PI*sin(glfwGetTime());
		//m[2] = sin(glfwGetTime())/2+0.5;
		

		wip_globj_t normalTransform;
		{
			wip_globj_t go;
			mat4x4_invert(go.m, transform->m);
			mat4x4_transpose(normalTransform.m, go.m);
		}

		wip_globj_t mpv;
		mat4x4_mul(mpv.m, projection, view);
		mat4x4_mul(mpv.m, mpv.m, transform->m);

		glUseProgram(program);
		
		glUniform3fv(lightLocation, 1, light.position);
		glUniformMatrix4fv(mpvLocation, 1, GL_FALSE, mpv.f);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transform->f);
		glUniformMatrix4fv(normalTransformLocation, 1, GL_FALSE, normalTransform.f);
		glUniform3fv(materialLocation, 1, m);

		glBindVertexArray(currentGlmdl->vertex_a);
		glDrawElements(GL_TRIANGLES, currentGlmdl->element_c, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		
		glUseProgram(program2);

		glUniformMatrix4fv(mpvLocation2, 1, GL_FALSE, mpv.f);
		glUniform1f(outlineThicknessLocation2, 0.02);
		glUniform3fv(outlineColorLocation2, 1, (float []){0.1, 0, 0});

		glCullFace(GL_FRONT);
		glBindVertexArray(currentGlmdl->vertex_a);
		glDrawElements(GL_TRIANGLES, currentGlmdl->element_c, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glCullFace(GL_BACK);


		glfwSwapBuffers(window);
		//glfwSetWindowShouldClose(window, GLFW_TRUE);
		glfwPollEvents();
	}

	wip_free(transform);
	wip_free(rocket);

	glfwTerminate();
	return 0;
}

