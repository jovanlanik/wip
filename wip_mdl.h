//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Model Functions

#pragma once

#include "include/wip_types.h"

typedef struct {
	float *vertex;
	float *normal;
	uint8_t *color;
	uint32_t *index;
	int vertex_c;
	int index_c;
} wip_ply_t;

typedef struct {
	struct {
		struct NAMED_VEC(3, float, XYZ, vertex, v);
		struct NAMED_VEC(3, float, XYZ, normal, n);
		struct NAMED_VEC(4, uint8_t, RGBA, color, c);
	} *model;
	uint32_t *index;
	int *vertex_c;
	int *index_c;
} wip_mdl_t;

typedef struct {
	#define BUFF2(type) { type data_b; type element_b; }
	union NAMED_VEC(2, GLuint, BUFF2, buffers, );
	#undef BUFF2
	size_t element_c;
	GLuint vertex_a;
} wip_glmdl_t;

wip_ply_t *wip_readModel(wip_ply_t *p, char *file);
wip_mdl_t *wip_prepModel(wip_mdl_t *m, wip_ply_t *p);
wip_glmdl_t *wip_loadModel(wip_glmdl_t *gm, wip_mdl_t *m);

