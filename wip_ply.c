//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// PLY model loading

#include "wip_ply.h"
#include "wip_fn.h"
#define MSH_PLY_IMPLEMENTATION
#include "msh_ply.h"

wip_modelPly *wip_loadModelPly(wip_modelPly *m, char *file) {
	wip_debug(WIP_INFO, "%s: Loading model from %s", __func__, file);

	m->vertData = NULL;
	m->vertCount = 0;
	m->indxData = NULL;
	m->indxCount = 0;

	msh_ply_desc_t wip_modelFormatVert = {
		.element_name = "vertex",
		.property_names = (const char*[]){"x", "y", "z"},
		.num_properties = 3,
		.data_type = MSH_PLY_FLOAT
	};

	msh_ply_desc_t wip_modelFormatNormal = {
		.element_name = "vertex",
		.property_names = (const char*[]){"nx", "ny", "nz"},
		.num_properties = 3,
		.data_type = MSH_PLY_FLOAT
	};

	msh_ply_desc_t wip_modelFormatColor = {
		.element_name = "vertex",
		.property_names = (const char*[]){"red", "green", "blue", "alpha"},
		.num_properties = 4,
		.data_type = MSH_PLY_UINT8
	};

	msh_ply_desc_t wip_modelFormatFace = {
		.element_name = "face",
		.property_names = (const char*[]){"vertex_indices"},
		.num_properties = 1,
		.data_type = MSH_PLY_UINT32,
		.list_type = MSH_PLY_UINT8,
		.list_size_hint = 3
	};

	wip_modelFormatVert.data = &m->vertData;
	wip_modelFormatVert.data_count = &m->vertCount;

	wip_modelFormatColor.data = &m->vertColor;
	wip_modelFormatColor.data_count = &m->vertCount;

	wip_modelFormatNormal.data = &m->vertNormal;
	wip_modelFormatNormal.data_count = &m->vertCount;

	wip_modelFormatFace.data = &m->indxData;
	wip_modelFormatFace.data_count = &m->indxCount;

	int r = 0;
	msh_ply_t *ply_file = msh_ply_open(file, "r");
	if(!ply_file) wip_log(WIP_FATAL, "%s: Couldn't open model %s", __func__, file);
	r = msh_ply_add_descriptor(ply_file, &wip_modelFormatVert);
	r = msh_ply_add_descriptor(ply_file, &wip_modelFormatColor);
	r = msh_ply_add_descriptor(ply_file, &wip_modelFormatNormal);
	r = msh_ply_add_descriptor(ply_file, &wip_modelFormatFace);
	r = msh_ply_read(ply_file);
	if(r) wip_log(WIP_FATAL, "%s: Couldn't parse model: %s", __func__, msh_ply_error_msg(r));
	msh_ply_close(ply_file);

	wip_modelFormatVert.data = NULL;
	wip_modelFormatVert.data_count = NULL;
	wip_modelFormatFace.data = NULL;
	wip_modelFormatFace.data_count = NULL;

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return m;
}

