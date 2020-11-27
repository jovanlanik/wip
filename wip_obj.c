//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Object Functions

#include <string.h>

#include "wip_fn.h"
#include "wip_obj.h"
#include "wip_math.h"
#include "lib/linmath.h"

wip_obj_t *wip_makeObject(wip_obj_t *o) {
	if(o) {
		memset(o, 0, sizeof(wip_obj_t));
		memcpy(o->scale, (float []){1.0, 1.0, 1.0}, sizeof(o->scale));
	}
	return o;
}

wip_globj_t *wip_loadObjectF(enum wip_objFlags f, wip_globj_t *go, wip_obj_t *o) {
	mat4x4 matrix;
	mat4x4_identity(matrix);
	if((WIP_SCALE & f) == WIP_SCALE) mat4x4_scale_aniso(matrix, matrix, o->s.x, o->s.y, o->s.z);
	if((WIP_POS & f) == WIP_POS) mat4x4_translate(matrix, o->x, o->y, o->z);
	// TODO: fix this shit
	if((WIP_ROT & f) == WIP_ROT) {
		mat4x4_rotate_Y(matrix, matrix, RAD(o->r.y));
		mat4x4_rotate_X(matrix, matrix, RAD(o->r.x));
		mat4x4_rotate_Z(matrix, matrix, RAD(o->r.z));
	}
	memcpy(go->m, matrix, sizeof(mat4x4));
	return go;
}

