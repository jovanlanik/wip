//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Fragment Shader

#version 330

in vec3 nor;
in vec3 pos;
in vec4 col;

uniform vec3 light;
uniform mat4 transform;
uniform vec3 material;

out vec4 fragColor;

void main() {
	vec3 tNor = mat3(transpose(inverse(transform))) * nor;
	vec3 nLight = light - vec3(transform * vec4(pos, 1.0f));
	float value;
	value = float(dot(tNor, nLight));
	value = smoothstep(material[0], material[0] + material[1], value);
	value = clamp(value, material[2], 1.0);
	fragColor = vec4(value * col.rgb, col.a);
}

