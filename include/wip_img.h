// WIP
// Copyright (c) Jovan Lanik 2021

// Image Loading

#pragma once

typedef struct {
	int width;
	int height;
	int channels;
	unsigned char *data;
} wip_img_t;

wip_img_t *wip_openImage(wip_img_t *image, const char *filename);
int wip_freeImage(wip_img_t *image);
GLuint wip_loadTexture(wip_img_t *image);

