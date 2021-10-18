// Starfield
// Copyright (c) 2021 Jovan Lanik

// Starfield Vertex Shader

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 2) in vec2 inUv;

uniform mat4 mpv;

out vec2 uv;

void main() {
	gl_Position = mpv * vec4(inPos, 1.0f);
	uv = inUv;
}

