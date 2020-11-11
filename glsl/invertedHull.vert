//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Inverted Hull Shader

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNor;

uniform mat4 mpv;
uniform float outlineThickness;

void main() {
	vec3 pos = inPos + inNor * outlineThickness;
	gl_Position = mpv * vec4(pos, 1.0f);
}

