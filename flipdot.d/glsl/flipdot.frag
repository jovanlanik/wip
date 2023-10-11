// Flipdot
// Copyright (c) 2023 Jovan Lanik

// Fragment Shader

#version 330

in vec3 inPos;

uniform float time;
uniform sampler2D imageTexture;

out vec4 fragColor;

float doCircle(vec2 uv, float x, float y, float size) {
	return floor(length(uv+vec2(x, y)) - size + 0.988);
}

float doRing(vec2 uv, float x, float y, float size, float thicc) {
	float ring = length(uv+vec2(x, y)) - size;
	ring = abs(ring) * thicc;
	ring = floor(ring);
	return ring;
}

float doBrm(vec2 uv, float x, float y, float size) {
	float ringSize = 0.425;
	float detailThicc = 80.0;

	uv += vec2(x, y);
	uv *= size;

	float ring = doRing(uv, 0.0, 0.0, ringSize, 40.0);

	float small = doRing(uv, 0.0, 0.0, ringSize - 0.1, detailThicc);

	small = max(small, 1.0 - doCircle(uv, 0.0, -0.33, 0.07));
	small = min(small, doRing(uv, 0.0, -0.33, 0.07, detailThicc));

	small = max(small, 1.0 - doCircle(uv, 0.25, 0.21, 0.07));
	small = min(small, doRing(uv, 0.25, 0.21, 0.07, detailThicc));

	small = max(small, 1.0 - doCircle(uv, -0.25, 0.21, 0.07));
	small = min(small, doRing(uv, -0.25, 0.21, 0.07, detailThicc));

	small = max(small, doCircle(uv, 0.0, 0.0, ringSize - 0.1));

	vec2 boxV = uv * 1.0;
	boxV = abs(boxV) * 1.0;
	boxV -= 0.04;
	boxV.x -= 0.04;
	boxV = abs(boxV) * 80.0;
	boxV = floor(boxV);
	boxV.x = max(boxV.x, floor(abs(uv.y)*19.0));
	boxV.y = max(boxV.y, floor(abs(uv.x)*12.0));
	float box = min(boxV.x, boxV.y);

	float put = min(ring, small);
	put = min(put, box);
	put = min(put, doRing(uv, 0.0, -0.16, 0.035, detailThicc));
	put = min(put, doRing(uv, 0.15, -0.16, 0.035, detailThicc));
	put = min(put, doRing(uv, -0.15, -0.16, 0.035, detailThicc));

	put = min(put, doRing(uv, 0.20, 0.05, 0.035, detailThicc));
	put = min(put, doRing(uv, -0.20, 0.05, 0.035, detailThicc));

	put = min(put, doRing(uv, 0.08, 0.20, 0.035, detailThicc));
	put = min(put, doRing(uv, -0.08, 0.20, 0.035, detailThicc));

	return put;
}

void main() {
	vec2 uv = gl_FragCoord.xy * 0.01;

	// TAKT

	uv *= 1;
	float var = (sin(time*20.0)+1.0)*0.02 + 1.0;;
	uv *= -5;
	uv.x *= -1;
	uv *= var;
	uv.x += time*0.4;
	fragColor = texture(imageTexture, uv);

	// BRMLAB

	/*
	float var = (sin(time*20.0)+1.0)*0.1 + 1.0;;
	uv.x -= 0.5;
	uv *= 2;
	float put = doBrm(uv, 0.0, 0.0, var);
	uv.y *= 4;
	put = 1.0 - put;
	fragColor = vec4(put, put, put, 1.0);
	*/
}

