//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Object Functions

#include <stdlib.h>
#include <string.h>

#include "wip_obj.h"
#include "wip_fn.h"
#include "lib/linmath.h"

#define M_PI 3.14159265358979323846
#define	DEG(rad) (rad*180.0/M_PI)
#define RAD(deg) (deg*M_PI/180.0)

wip_obj_t *wip_makeObject(wip_obj_t *o) {
	if(o) {
		memset(o, 0, sizeof(wip_obj_t));
		memcpy(o->scale, (float []){1.0, 1.0, 1.0}, sizeof(o->scale));
	}
	return o;
}

wip_globj_t *wip_loadObject(wip_globj_t *go, wip_obj_t *o) {
	mat4x4 matrix;
	mat4x4_identity(matrix);
	mat4x4_scale_aniso(matrix, matrix, o->s.x, o->s.y, o->s.z);
	mat4x4_translate(matrix, o->x, o->y, o->z);
	// TODO: fix this shit
	mat4x4_rotate_Y(matrix, matrix, RAD(o->r.y));
	mat4x4_rotate_X(matrix, matrix, RAD(o->r.x));
	mat4x4_rotate_Z(matrix, matrix, RAD(o->r.z));
	memcpy(go->matrix, matrix, sizeof(mat4x4));
	return go;
}

