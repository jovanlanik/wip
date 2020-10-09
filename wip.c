//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Main Source

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <graphene-1.0/graphene.h>
//#include <pthread.h>

#include "funcs.h"
#include "shaders.h"
#include "model.h"

int main(int argc, char *argv[]) {
	wip_debug(WIP_INFO, "WIP built %s %s", __DATE__, __TIME__);

	GLFWwindow *window = wip_initWindow(argv[0]);

	GLuint program = glCreateProgram();

	GLuint vertShader = wip_loadShader(vert_glsl, GL_VERTEX_SHADER);
	glAttachShader(program, vertShader);

	GLuint fragShader = wip_loadShader(frag_glsl, GL_FRAGMENT_SHADER);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	GLuint posBuff, colBuff, vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &posBuff);
	glGenBuffers(1, &colBuff);
	glBindBuffer(GL_ARRAY_BUFFER, posBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, colBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model_color), model_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
	glEnableVertexAttribArray(1);

	float transformFloat[16];
	unsigned int transformLocation = glGetUniformLocation(program, "transform");
	graphene_matrix_t *transform = graphene_matrix_init_identity(graphene_matrix_alloc());
	graphene_matrix_scale(transform, 0.5f, 0.5f, 0.5f);

	float time = 0;
	unsigned int timeLocation = glGetUniformLocation(program, "time");

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		graphene_matrix_rotate_z(transform, 1.0f);
		graphene_matrix_to_float(transform, transformFloat);
		time = (float)(glfwGetTime()/4 - (int)glfwGetTime()/4);
		glUseProgram(program);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transformFloat);
		glUniform1f(timeLocation, time);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

