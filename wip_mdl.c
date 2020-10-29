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

	msh_ply_desc_t modelFormatVertex = {
		.element_name = "vertex",
		.property_names = (const char*[]){"x", "y", "z"},
		.num_properties = 3,
		.data_type = MSH_PLY_FLOAT
	};

	msh_ply_desc_t modelFormatNormal = {
		.element_name = "vertex",
		.property_names = (const char*[]){"nx", "ny", "nz"},
		.num_properties = 3,
		.data_type = MSH_PLY_FLOAT
	};

	msh_ply_desc_t modelFormatColor = {
		.element_name = "vertex",
		.property_names = (const char*[]){"red", "green", "blue", "alpha"},
		.num_properties = 4,
		.data_type = MSH_PLY_UINT8
	};

	msh_ply_desc_t modelFormatFace = {
		.element_name = "face",
		.property_names = (const char*[]){"vertex_indices"},
		.num_properties = 1,
		.data_type = MSH_PLY_UINT32,
		.list_type = MSH_PLY_UINT8,
		.list_size_hint = 3
	};

	modelFormatVertex.data = &p->vertex;
	modelFormatVertex.data_count = &p->vertex_c;

	int color_c, normal_c;

	modelFormatColor.data = &p->color;
	modelFormatColor.data_count = &color_c;

	modelFormatNormal.data = &p->normal;
	modelFormatNormal.data_count = &normal_c;

	modelFormatFace.data = &p->index;
	modelFormatFace.data_count = &p->index_c;

	bool hasColor = true, hasNormals = true;
	int r = 0;
	msh_ply_t *ply_file = msh_ply_open(file, "r");
	if(!ply_file) wip_log(WIP_FATAL, "%s: Couldn't open model %s", __func__, file);
	r = msh_ply_parse_header(ply_file);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't parse header: %s", __func__, msh_ply_error_msg(r));
	r = msh_ply_add_descriptor(ply_file, &modelFormatVertex);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));
	if(msh_ply_has_properties(ply_file, &modelFormatColor))
		msh_ply_add_descriptor(ply_file, &modelFormatColor);
	else
		hasColor = false;
	if(msh_ply_has_properties(ply_file, &modelFormatNormal))
		msh_ply_add_descriptor(ply_file, &modelFormatNormal);
	else
		hasNormals = false;
	r = msh_ply_add_descriptor(ply_file, &modelFormatFace);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));
	r = msh_ply_read(ply_file);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));
	wip_free(msh_ply_find_element(ply_file, "vertex")->data);
	wip_free(msh_ply_find_element(ply_file, "face")->data);
	msh_ply_close(ply_file);
	
	/*
	for(int i = 0; i < *modelFormatVertex.data_count; ++i) {
		modelFormatColor.data[i] = 255;
		modelFormatNormal.data[i] = 1.0;
	}
	*/


	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return p;
}

