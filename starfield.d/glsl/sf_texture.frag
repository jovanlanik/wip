//
// Starfield
// Copyright (c) 2021 Jovan Lanik
//

// Starfield Fragment Shader

#version 330

in vec2 uv;

uniform sampler2D imageTexture;

out vec4 fragColor;

void main() {
	fragColor = texture(imageTexture, uv);
}

