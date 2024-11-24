// WIP
// Copyright (c) Jovan Lanik 2020

// Object Functions

#pragma once

#include "wip_types.h"

enum wip_objFlags {
	WIP_POS = 1,
	WIP_ROT = 2,
	WIP_SCALE = 4,
};

typedef struct {
	union WIP_NAMED_VEC_T(3, float, WIP_XYZ, position, );
	union WIP_NAMED_VEC_T(3, float, WIP_XYZ, scale, s);
	union WIP_NAMED_VEC_T(4, float, WIP_XYZW, rotation, r);
	//float rotation[4];
	//union WIP_NAMED_VEC_T(3, float, WIP_XYZ, momentum, m);
	//union WIP_NAMED_VEC_T(3, float, WIP_XYZ, angular, a);
} wip_obj_t;

typedef union WIP_NAMED_MAT_T(4, 4, float, f, m) wip_globj_t;

typedef struct {
	wip_obj_t **object;
	int length;
} wip_scene_t;

wip_obj_t *wip_makeObject(wip_obj_t *o);
#define wip_loadObject(go, o) wip_loadObjectF(WIP_POS|WIP_ROT|WIP_SCALE, go, o)
wip_globj_t *wip_loadObjectF(enum wip_objFlags, wip_globj_t *go, wip_obj_t *o);
void wip_unloadObject(wip_globj_t *go);

