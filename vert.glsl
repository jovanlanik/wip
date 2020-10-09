//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Vertex Shader

#version 330

in vec3 inPos;

void main() {
	gl_Position = vec4(inPos, 1.0f);
}
