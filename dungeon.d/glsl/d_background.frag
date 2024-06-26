// Dungeon
// Copyright (c) 2023 Jovan Lanik

// Background cloud shader

#version 330

in vec3 inPos;

uniform float dir;
uniform float time;

out vec4 fragColor;

vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v) {
	const vec4 C = vec4(0.211324865405187, 0.366025403784439,
		-0.577350269189626, 0.024390243902439);
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);
	vec2 i1;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	i = mod(i, 289.0);
	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));
	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
			dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;
	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
	vec3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}

void main() {
	vec2 ouv = gl_FragCoord.xy * 0.001;

	vec2 uv = ouv;
	uv *= 1.0;
	uv.x += sin(uv.y*60.0 + sin(time/2.0)) / 40.0;
	uv.y *= 3.0;

	float t = 0.01;
	float c = 0.1;

	vec2 uv1 = uv;
	uv1.x += c *sin(dir) + time*t;
	uv1.y += 0.3;
	vec2 uv2 = uv;
	uv2.x -= c * sin(dir) + time*(t/2.0);
	uv2.y -= 0.5;

	float n1 = snoise((uv1)*5.0)*0.5+0.5;
	float n2 = snoise((uv2)*5.0)*0.5+0.5;

	float x = 0.55;
	float d = 0.975;
	float n = step(ouv.y - x, n1 * n2);
	vec3 bg1 = vec3(0.725, 0.825, 0.975);
	vec3 bg2 = bg1 * d;

	vec3 col = mix(bg1, bg2, n);
	fragColor = vec4(col,1.0);
}
