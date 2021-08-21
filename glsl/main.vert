//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Vertex Shader

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 3) in vec4 inCol;

uniform mat4 mpv;

out vec3 nor;
out vec3 pos;
out vec4 col;

void main() {
	gl_Position = mpv * vec4(inPos, 1.0f);
	nor = inNor;
	pos = inPos;
	col = inCol;
}

