// WIP
// Copyright (c) 2021 Jovan Lanik

// Text Fragment Shader

#version 330

in vec2 uv;

uniform uint c;
uniform sampler2D imageTexture;

out vec4 fragColor;

void main() {
	const vec2 grid = vec2(32, 3);
	const vec2 pixel = 1.0f / grid;
	vec2 cUv;
	cUv = uv;
	cUv *= pixel;
	cUv.y -= 1.0f / 3.0f;
	cUv.x += float(c % 32u) * pixel.x;
	cUv.y -= float(c / 32u - 1u) * pixel.y;
	fragColor = texture(imageTexture, cUv);
}

