// Flipdot demo
// Copyright (c) 2023 Jovan Lanik

// Vertex Shader

#version 330

in vec3 inPos;

out vec3 outPos;

void main() {
	gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);
	outPos = inPos;
}

