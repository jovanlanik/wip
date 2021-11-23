// WIP
// Copyright (c) 2020 Jovan Lanik

// Model Functions

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "wip_types.h"

typedef struct {
	int vertex_c;
	int index_c;
	float *vertex;
	float *normal;
	float *uv;
	uint8_t *color;
	uint32_t *index;
} wip_ply_t;

typedef struct {
	struct {
		struct WIP_NAMED_VEC_T(3, float, WIP_XYZ, vertex, v);
		struct WIP_NAMED_VEC_T(3, float, WIP_XYZ, normal, n);
		struct WIP_NAMED_VEC_T(2, float, WIP_UV, uv, u);
		struct WIP_NAMED_VEC_T(4, uint8_t, WIP_RGBA, color, c);
	} *model;
	uint32_t *index;
	int vertex_c;
	int index_c;
} wip_mdl_t;

wip_ply_t *wip_readModel(wip_ply_t *p, const char *file);
wip_mdl_t *wip_prepModel(wip_mdl_t *m, wip_ply_t *p);

#ifdef GL_TRUE
#define UINT GLuint
#else
#define UINT uint32_t
#endif
#define BUFF2(type) { type data_b; type element_b; }

typedef struct {
	union WIP_NAMED_VEC_T(2, UINT, BUFF2, buffers, );
	size_t element_c;
	UINT vertex_a;
} wip_glmdl_t;

#undef UINT
#undef BUFF2

wip_glmdl_t *wip_loadModel(wip_glmdl_t *gm, wip_mdl_t *m);
wip_glmdl_t *wip_openModel(char *n);
