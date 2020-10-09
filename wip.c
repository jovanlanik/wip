//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Main Source

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <graphene-1.0/graphene.h>
//#include <pthread.h>

#include "funcs.h"
#include "shaders.h"
#include "model.h"

int main(int argc, char *argv[]) {
	wip_debug(WIP_INFO, "WIP built %s %s", __DATE__, __TIME__);

	GLFWwindow *window = wip_initWindow(argv[0]);

	GLuint program = glCreateProgram();
	GLuint program2 = glCreateProgram();

	GLuint vertShader = wip_loadShader(vert_glsl, GL_VERTEX_SHADER);
	glAttachShader(program, vertShader);
	glAttachShader(program2, vertShader);

	GLuint fragShader = wip_loadShader(frag_glsl, GL_FRAGMENT_SHADER);
	glAttachShader(program, fragShader);
	GLuint frag2Shader = wip_loadShader(frag2_glsl, GL_FRAGMENT_SHADER);
	glAttachShader(program2, frag2Shader);

	glLinkProgram(program);
	glLinkProgram(program2);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(frag2Shader);

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUseProgram(program2);
		glDrawArrays(GL_TRIANGLES, 6, 3);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

