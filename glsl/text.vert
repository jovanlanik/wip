// WIP
// Copyright (c) 2021 Jovan Lanik

// Text Vertex Shader
// vim: filetype=c

#version 330

layout (location = 0) in vec3 inPos;
layout (location = 2) in vec2 inUv;

uniform uvec2 screen;
uniform uvec2 position;
uniform uvec2 offset;
uniform float scale;

out vec2 uv;

void main() {
	const vec2 charSize = vec2(7, 13);
	vec2 rPixel = 1.0f / screen;
	vec2 sPixel = scale / screen;
	vec2 outPos;
	outPos = inPos.xy;
	outPos *= sPixel * charSize;
	outPos.x += rPixel.x * position.x;
	outPos.y -= rPixel.y * position.y;
	outPos += vec2(-1.0f, 1.0f);
	outPos.x += charSize.x * sPixel.x;
	outPos.y -= charSize.y * sPixel.y;
	outPos.x += 2.0f * offset.x * charSize.x * sPixel.x;
	outPos.y -= 2.0f * offset.y * charSize.y * sPixel.y;
	gl_Position = vec4(outPos, 0.0f, 1.0f);
	uv = inUv;
}

