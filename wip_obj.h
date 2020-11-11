//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Object Functions

#pragma once

#include "wip_types.h"

typedef struct {
	union POS;
	union NAMED_VEC(3, float, XYZ, rotation, r);
	union NAMED_VEC(3, float, XYZ, scale, s);
	union NAMED_VEC(3, float, XYZ, momentum, m);
	union NAMED_VEC(3, float, XYZ, angular, a);
} wip_obj_t;

typedef union NAMED_MAT(4, 4, float, f, m) wip_globj_t;

wip_obj_t *wip_makeObject(wip_obj_t *o);
wip_globj_t *wip_loadObject(wip_globj_t *go, wip_obj_t *o);
void wip_unloadObject(wip_globj_t *go);

