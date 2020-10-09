//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Vertex Shader

#version 330

in vec3 inPos;
in vec3 inCol;
uniform mat4 transform;
out vec3 ourCol;

void main() {
	gl_Position = transform * vec4(inPos, 1.0f);
	ourCol = inCol;
}
