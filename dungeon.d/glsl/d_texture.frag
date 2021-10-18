// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon Fragment Shader

#version 330

in vec3 nor;
in vec2 uv;

uniform vec3 light;
uniform mat4 transform;
uniform mat4 normalTransform;
uniform vec3 material;
uniform sampler2D imageTexture;

out vec4 fragColor;

void main() {
	vec3 tNor;
	tNor = mat3(normalTransform) * nor;
	tNor = normalize(tNor);
	vec3 nLight;
	nLight = light - vec3(transform);
	nLight = normalize(nLight);
	float value;
	value = float(dot(tNor, nLight));
	value = smoothstep(material[0], material[0] + material[1], value);
	value = clamp(value, material[2], 1.0);
	vec4 col;
	col = texture(imageTexture, uv);
	fragColor = vec4(value * col.rgb, col.a);
}

