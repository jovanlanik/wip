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
	vec3 tNor = mat3(transpose(inverse(transform))) * nor;
	float value = float(dot(tNor, light));
	float l = 0.1;
	float d = 0.1;
	float m = 0.4;
	value = smoothstep(l, l + d, value);
	value = clamp(value, m, 1.0);

	fragColor = vec4(value * col.r, value * col.g, value * col.b, 1.0);
	//fragColor = vec4(value, value, value, 1.0);
	//fragColor = vec4(nor+0.5, 1.0);
}

