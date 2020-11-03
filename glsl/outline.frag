//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Outline Shader

#version 330

uniform vec3 outlineColor;

out vec4 fragColor;

void main() {
	fragColor = vec4(outlineColor, 1.0f);
}

