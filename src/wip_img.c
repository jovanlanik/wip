// WIP
// Copyright (c) Jovan Lanik 2021

// Image Loading

#include "wip_fn.h"
#include "wip_img.h"
#ifndef NDEBUG
#define STBI_MALLOC(x) wip_alloc(x)
#define STBI_REALLOC(x, y) wip_realloc(x, y, NULL)
#define STBI_FREE(x) wip_free(x)
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

wip_img_t *wip_openImage(wip_img_t *image, const char *filename) {
	stbi_set_flip_vertically_on_load(1); // TODO: I wonder if this is the best place to do this...
	wip_debug(WIP_INFO, "%s: Loading image from %s...", __func__, filename);
	image->data = stbi_load(filename, &image->width, &image->height, &image->channels, 0);
	if(!image->data) wip_log(WIP_ERROR, "%s: Couldn't open image %s", __func__, filename);
	else wip_debug(WIP_INFO, "%s: Done.", __func__);
	return image;
}

int wip_freeImage(wip_img_t *image) {
	// TODO: treat failure
	stbi_image_free(image->data);
	return 1;
}

GLuint wip_loadTexture(wip_img_t *image) {
	// TODO: loading multiple textures
	// TODO: treat failure
	GLuint texture;
	GLenum format = image->channels == 4 ? GL_RGBA : GL_RGB;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data);
	//glGenerateMipmap(GL_TEXTURE_2D);
	return texture;
}

// TODO: error
GLuint wip_openTexture(char *name) {
	GLuint texture = 0;
	wip_img_t image;
	
	// TODO: resource finding function
	const char *prefix = "./res/img/";
	const char *suffix = ".png";
	char *filename = wip_alloc(strlen(prefix)+strlen(name)+strlen(suffix)+1);
	sprintf(filename, "%s%s%s", prefix, name, suffix);

	wip_openImage(&image, filename);
	wip_free(filename);

	if(image.data) {
		texture = wip_loadTexture(&image);
		wip_freeImage(&image);
	}

	return texture;
}

// TODO: unload or free texture

