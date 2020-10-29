//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// PLY model loading

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "wip_mdl.h"
#include "wip_fn.h"
#define MSH_PLY_MALLOC(x) wip_alloc(x)
#define MSH_PLY_REALLOC(x, y) wip_realloc(x, y)
#define MSH_PLY_FREE(x) wip_free(x)
#define MSH_PLY_IMPLEMENTATION
#include "lib/msh_ply.h"

wip_ply_t *wip_readModel(wip_ply_t *p, char *file) {
	wip_debug(WIP_INFO, "%s: Loading model from %s", __func__, file);


	msh_ply_desc_t formatVertex = {
		.element_name = "vertex",
		.property_names = (const char*[]){"x", "y", "z"},
		.num_properties = 3,
		.data_type = MSH_PLY_FLOAT
	};

	msh_ply_desc_t formatNormal = {
		.element_name = "vertex",
		.property_names = (const char*[]){"nx", "ny", "nz"},
		.num_properties = 3,
		.data_type = MSH_PLY_FLOAT
	};

	msh_ply_desc_t formatColor = {
		.element_name = "vertex",
		.property_names = (const char*[]){"red", "green", "blue", "alpha"},
		.num_properties = 4,
		.data_type = MSH_PLY_UINT8
	};

	msh_ply_desc_t formatFace = {
		.element_name = "face",
		.property_names = (const char*[]){"vertex_indices"},
		.num_properties = 1,
		.data_type = MSH_PLY_UINT32,
		.list_type = MSH_PLY_UINT8,
		.list_size_hint = 3
	};

	formatVertex.data = &p->vertex;
	formatVertex.data_count = &p->vertex_c;

	int color_c, normal_c;

	formatColor.data = &p->color;
	formatColor.data_count = &color_c;

	formatNormal.data = &p->normal;
	formatNormal.data_count = &normal_c;

	formatFace.data = &p->index;
	formatFace.data_count = &p->index_c;


	bool hasColor = true, hasNormal = true;
	int r = 0;

	msh_ply_t *ply_file = msh_ply_open(file, "r");
	if(!ply_file) wip_log(WIP_FATAL, "%s: Couldn't open model %s", __func__, file);

	r = msh_ply_parse_header(ply_file);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't parse header: %s", __func__, msh_ply_error_msg(r));

	r = msh_ply_add_descriptor(ply_file, &formatVertex);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));

	if(msh_ply_has_properties(ply_file, &formatColor))
		msh_ply_add_descriptor(ply_file, &formatColor);
	else
		hasColor = false;
	if(msh_ply_has_properties(ply_file, &formatNormal))
		msh_ply_add_descriptor(ply_file, &formatNormal);
	else
		hasNormal = false;

	r = msh_ply_add_descriptor(ply_file, &formatFace);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));

	r = msh_ply_read(ply_file);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));

	wip_free(msh_ply_find_element(ply_file, "vertex")->data);
	wip_free(msh_ply_find_element(ply_file, "face")->data);
	msh_ply_close(ply_file);
	
	if(!hasColor) 
		*((void **)formatColor.data) = wip_alloc((int)*formatVertex.data_count * 4 * sizeof(uint8_t));
	if(!hasNormal) 
		*((void **)formatNormal.data) = wip_alloc((int)*formatVertex.data_count * 3 * sizeof(float));

	for(int i = 0; i < (int)*formatVertex.data_count; ++i) {
		if(!hasColor) {
			uint8_t *colorData = *(void **)formatColor.data;
			memcpy(
				&colorData[4*i],
				(uint8_t []){255, 255, 255, 255},
				4 * sizeof(uint8_t)
			);
		}
		if(!hasNormal) {
			// TODO: calculate normals here
			float *normalData = *(void **)formatNormal.data;
			memcpy(
				&normalData[3*i],
				(float []){0, 0, 0},
				3 * sizeof(float)
			);
		}
			
	}


	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return p;
}

