//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Main Source

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <graphene-1.0/graphene.h>
//#include <pthread.h>

#include "wip_fn.h"
#include "wip_gl.h"
#include "wip_ply.h"
#include "wip_glfw.h"

#include "shaders.h"

int main(int argc, char *argv[]) {
	wip_debug(WIP_INFO, "WIP built %s %s", __DATE__, __TIME__);
	GLFWwindow *window = wip_initWindow(argv[0]);
	wip_glInit();


	GLuint vertShader = wip_loadShader((GLchar*)glsl_main_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((GLchar*)glsl_main_frag, GL_FRAGMENT_SHADER);

	GLuint vert2Shader = wip_loadShader((GLchar*)glsl_invertedHull_vert, GL_VERTEX_SHADER);
	GLuint frag2Shader = wip_loadShader((GLchar*)glsl_outline_frag, GL_FRAGMENT_SHADER);

	GLuint program = wip_mkProgram(vertShader, fragShader);
	GLuint program2 = wip_mkProgram(vert2Shader, frag2Shader);

	glDeleteShader(vertShader);
	glDeleteShader(vert2Shader);
	glDeleteShader(fragShader);
	glDeleteShader(frag2Shader);


	wip_modelPly model;
	wip_loadModelPly(&model, "mdl/wip_model.ply");

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
	glBufferData(GL_ARRAY_BUFFER, model.vertCount*3*sizeof(model.vertData[0]), model.vertData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, norBuff);
	glBufferData(GL_ARRAY_BUFFER, model.vertCount*3*sizeof(model.vertNormal[0]), model.vertNormal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, colBuff);
	glBufferData(GL_ARRAY_BUFFER, model.vertCount*4*sizeof(model.vertColor[0]), model.vertColor, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indxCount*3*sizeof(model.indxData[0]), model.indxData, GL_STATIC_DRAW);


	float rot_x = 0;
	float rot_y = 0;
	float rot_z = 0;
	
	float lightFloat[3];
	float transformFloat[16];
	float viewFloat[16];
	float projectionFloat[16];

	unsigned int lightLocation = glGetUniformLocation(program, "light");
	unsigned int transformLocation = glGetUniformLocation(program, "transform");
	unsigned int projectionLocation = glGetUniformLocation(program, "projection");
	unsigned int viewLocation = glGetUniformLocation(program, "view");

	unsigned int transformLocation2 = glGetUniformLocation(program2, "transform");
	unsigned int projectionLocation2 = glGetUniformLocation(program2, "projection");
	unsigned int viewLocation2 = glGetUniformLocation(program2, "view");

	graphene_vec3_t *light = graphene_vec3_init_from_float(graphene_vec3_alloc(), (float[]){-1.0f, 0.2f, 0.2f});
	graphene_vec3_normalize(light, light);
	
	graphene_vec3_t *eye = graphene_vec3_init_from_float(graphene_vec3_alloc(), (float[]){0.0f, 0.0f, 3.0f});
	graphene_vec3_t *center = graphene_vec3_init_from_float(graphene_vec3_alloc(), (float[]){0.0f, 0.0f, 0.0f});

	graphene_matrix_t *transform = graphene_matrix_alloc();
	graphene_matrix_t *view = graphene_matrix_init_look_at(graphene_matrix_alloc(), eye, center, graphene_vec3_y_axis());
	graphene_matrix_t *projection = graphene_matrix_init_perspective(graphene_matrix_alloc(), 90.0f, 16.0f/10.0f, 0.1f, 100.0f);

	graphene_vec3_to_float(light, lightFloat);
	graphene_matrix_to_float(view, viewFloat);
	graphene_matrix_to_float(projection, projectionFloat);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		rot_x = 15;
		//rot_x = 25*glfwGetTime();
		//rot_y = 90;
		rot_y = 25*glfwGetTime();
		//rot_z = 90;
		//rot_z = 25*glfwGetTime();
		float scale = 1.0f;


		glUseProgram(program2);

		transform = graphene_matrix_init_identity(transform);
		graphene_matrix_rotate_x(transform, rot_x);
		graphene_matrix_rotate_y(transform, rot_y);
		graphene_matrix_rotate_z(transform, rot_z);
		graphene_matrix_scale(transform, scale, scale, scale);
		graphene_matrix_to_float(transform, transformFloat);

		glUniformMatrix4fv(transformLocation2, 1, GL_FALSE, transformFloat);
		glUniformMatrix4fv(viewLocation2, 1, GL_FALSE, viewFloat);
		glUniformMatrix4fv(projectionLocation2, 1, GL_FALSE, projectionFloat);

		glCullFace(GL_FRONT);
		glDrawElements(GL_TRIANGLES, model.indxCount*3, GL_UNSIGNED_INT, 0);
		glCullFace(GL_BACK);


		glUseProgram(program);

		transform = graphene_matrix_init_identity(transform);
		graphene_matrix_rotate_x(transform, rot_x);
		graphene_matrix_rotate_y(transform, rot_y);
		graphene_matrix_rotate_z(transform, rot_z);
		graphene_matrix_scale(transform, scale, scale, scale);
		graphene_matrix_to_float(transform, transformFloat);
		
		glUniform3fv(lightLocation, 1, lightFloat);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transformFloat);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewFloat);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionFloat);

		glDrawElements(GL_TRIANGLES, model.indxCount*3, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

