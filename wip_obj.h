//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Object Functions

#pragma once

#include "wip_types.h"

typedef struct {
	union WIP_POS;
	union WIP_NAMED_VEC(3, float, WIP_XYZ, rotation, r);
	union WIP_NAMED_VEC(3, float, WIP_XYZ, scale, s);
	union WIP_NAMED_VEC(3, float, WIP_XYZ, momentum, m);
	union WIP_NAMED_VEC(3, float, WIP_XYZ, angular, a);
} wip_obj_t;

typedef union WIP_NAMED_MAT(4, 4, float, f, m) wip_globj_t;

typedef struct {
	wip_obj_t **object;
	int length;
} wip_scene_t;

wip_obj_t *wip_makeObject(wip_obj_t *o);
wip_globj_t *wip_loadObject(wip_globj_t *go, wip_obj_t *o);
void wip_unloadObject(wip_globj_t *go);

