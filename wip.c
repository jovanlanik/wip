//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Main Source

#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <pthread.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "wip_fn.h"
#include "wip_gl.h"
#include "wip_obj.h"
#include "wip_mdl.h"
#include "wip_glfw.h"
#include "lib/linmath.h"

#include "shaders.h"

#define M_PI 3.14159265358979323846
#define	DEG(rad) (rad*180.0/M_PI)
#define RAD(deg) (deg*M_PI/180.0)

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
	

	wip_ply_t model;
	wip_readModel(&model, "mdl/wip_model.ply");

	GLuint posBuff;
	GLuint norBuff;
	GLuint colBuff;
	GLuint ebo, vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &posBuff);
	glGenBuffers(1, &norBuff);
	glGenBuffers(1, &colBuff);
	glGenBuffers(1, &ebo);
	
	glBindBuffer(GL_ARRAY_BUFFER, posBuff);
	glBufferData(GL_ARRAY_BUFFER, model.vertex_c*3*sizeof(model.vertex[0]), model.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, norBuff);
	glBufferData(GL_ARRAY_BUFFER, model.vertex_c*3*sizeof(model.normal[0]), model.normal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, colBuff);
	glBufferData(GL_ARRAY_BUFFER, model.vertex_c*4*sizeof(model.color[0]), model.color, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.index_c*3*sizeof(model.index[0]), model.index, GL_STATIC_DRAW);


	wip_obj_t *rocket = wip_allocType(wip_obj_t);
	wip_makeObject(rocket);
	wip_globj_t *transformFloat = wip_allocType(wip_globj_t);


	unsigned int lightLocation = glGetUniformLocation(program, "light");
	unsigned int transformLocation = glGetUniformLocation(program, "transform");
	unsigned int projectionLocation = glGetUniformLocation(program, "projection");
	unsigned int viewLocation = glGetUniformLocation(program, "view");
	unsigned int materialLocation = glGetUniformLocation(program, "material");

	unsigned int transformLocation2 = glGetUniformLocation(program2, "transform");
	unsigned int projectionLocation2 = glGetUniformLocation(program2, "projection");
	unsigned int viewLocation2 = glGetUniformLocation(program2, "view");
	unsigned int outlineThicknessLocation2 = glGetUniformLocation(program2, "outlineThickness");
	unsigned int outlineColorLocation2 = glGetUniformLocation(program2, "outlineColor");

	float step = 0.2;

	wip_obj_t light, eye, center; 
	wip_makeObject(&light);
	wip_makeObject(&eye);
	wip_makeObject(&center);
	light.x = -2;
	light.y = -3;
	light.z = 2;
	
	vec3 axis = {0.0f, 0.0f, 1.0f};
	mat4x4 view;

	mat4x4 projection;
	mat4x4_perspective(projection, RAD(70), 16.0f/10.0f, 0.1, 100);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) eye.y += step;
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) eye.y -= step;
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) eye.x += step;
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) eye.x -= step;
		if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) eye.z += step;
		if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) eye.z -= step;
		center.x = eye.x;
		center.y = eye.y + 1;
		center.z = eye.z;
		mat4x4_look_at(view, eye.position, center.position, axis);

		rocket->z = 0.5*sin(glfwGetTime());
		rocket->r.y = 90;
		rocket->r.x = 25*glfwGetTime();
		rocket->s.x = 1.2;
		rocket->s.y = 1.2;
		rocket->s.z = 1.2;
		wip_loadObject(transformFloat, rocket);


		glUseProgram(program2);
		glUniformMatrix4fv(transformLocation2, 1, GL_FALSE, transformFloat->matrix);
		glUniformMatrix4fv(viewLocation2, 1, GL_FALSE, (const float*)&view);
		glUniformMatrix4fv(projectionLocation2, 1, GL_FALSE, (const float*)&projection);
		glUniform1f(outlineThicknessLocation2, 0.02);
		glUniform3fv(outlineColorLocation2, 1, (float []){0.1, 0, 0});

		glCullFace(GL_FRONT);
		glDrawElements(GL_TRIANGLES, model.index_c*3, GL_UNSIGNED_INT, 0);
		glCullFace(GL_BACK);


		glUseProgram(program);
		
		
		glUniform3fv(lightLocation, 1, light.position);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transformFloat->matrix);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (const float*)&view);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (const float*)&projection);
		glUniform3fv(materialLocation, 1, (float []){0.1, 0.15, 0.4});

		glDrawElements(GL_TRIANGLES, model.index_c*3, GL_UNSIGNED_INT, 0);
		
		
		glfwSwapBuffers(window);
		//glfwSetWindowShouldClose(window, GLFW_TRUE);
		glfwPollEvents();
	}

	wip_free(transformFloat);
	wip_free(rocket);

	wip_free(model.vertex);
	wip_free(model.index);
	wip_free(model.color);
	wip_free(model.normal);

	glfwTerminate();
	return 0;
}

