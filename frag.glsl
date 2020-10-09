//
// WIP
// Copyright (c) 2020 Jovan Lanik

// Fragment Shader

#version 330

in vec3 ourCol;
out vec4 fragColor;

void main() {
	fragColor = vec4(ourCol, 1.0f);
}
