// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon Vertex Shader

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;
layout (location = 2) in vec2 inUv;

uniform mat4 mpv;

out vec3 nor;
out vec2 uv;

void main() {
	gl_Position = mpv * vec4(inPos, 1.0f);
	nor = inNor;
	uv = inUv;
}

