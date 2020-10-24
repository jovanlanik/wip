//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Vertex Shader

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec4 inCol;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
out vec3 nor;
out vec4 col;

void main() {
	gl_Position = projection * view * transform * vec4(inPos, 1.0f);
	//gl_Position = projection * view * vec4(inPos, 1.0f);
	nor = inNor;
	col = inCol;
}
