//
// WIP
// Copyright (c) Jovan Lanik 2020
//

// Object Functions

typedef float vec3[3];

struct xyz {
	float x;
	float y;
	float z;
};

typedef struct {
	union {
		vec3 position;
		struct {
			float x;
			float y;
			float z;
		};
	};
	union {
		vec3 rotation;
		struct xyz r;
	};
	union {
		vec3 scale;
		struct xyz s;
	};
	union {
		vec3 momentum;
		struct xyz m;
	};
	union {
		vec3 angular;
		struct xyz a;
	};
} wip_obj_t;

typedef struct {
	float matrix[16];
} wip_globj_t;

wip_obj_t *wip_makeObject(wip_obj_t *o);
wip_globj_t *wip_loadObject(wip_globj_t *go, wip_obj_t *o);
void wip_unloadObject(wip_globj_t *go);

