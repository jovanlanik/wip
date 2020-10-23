//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Fragment Shader

#version 330

in vec3 nor;
in vec4 col;
uniform vec3 light;
uniform mat4 transform;
out vec4 fragColor;

void main() {
	vec3 alight = vec4(vec4(light, 0.0) * transform).xyz;
	float value = float(dot(alight, nor));
	fragColor = vec4(value * col.r, value * col.g, value * col.b, 1.0);
}
