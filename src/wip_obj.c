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
		quat_identity(o->rotation);
		o->s.x = 1.0f;
		o->s.y = 1.0f;
		o->s.z = 1.0f;
	}
	return o;
}

wip_globj_t *wip_loadObjectF(enum wip_objFlags f, wip_globj_t *go, wip_obj_t *o) {
	mat4x4 matrix, rotation, scale;
	mat4x4_identity(matrix);
	if((WIP_POS & f) == WIP_POS) mat4x4_translate(matrix, o->x, o->y, o->z);
	if((WIP_ROT & f) == WIP_ROT) {
		mat4x4_from_quat(rotation, o->rotation);
		mat4x4_mul(matrix, matrix, rotation);
	}
	if((WIP_SCALE & f) == WIP_SCALE) {
		mat4x4_identity(scale);
		mat4x4_scale_aniso(scale, scale, o->s.x, o->s.y, o->s.z);
		mat4x4_mul(matrix, matrix, scale);
	}
	memcpy(go->m, matrix, sizeof(mat4x4));
	return go;
}

