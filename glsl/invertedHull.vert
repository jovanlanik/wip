//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Inverted Hull Shader

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
	float thick = 0.02;
	vec3 nPos = inPos + inNor * thick;
	gl_Position = projection * view * transform * vec4(nPos, 1.0f);

	//gl_Position = projection * view * vec4(inPos, 1.0f);
}

