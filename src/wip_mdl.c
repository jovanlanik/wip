// WIP
// Copyright (c) 2020 Jovan Lanik

// PLY model loading

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "wip_fn.h"
#include "wip_mdl.h"
#include "external/glad/glad.h"
#define MSH_PLY_MALLOC(x) wip_alloc(x)
#define MSH_PLY_REALLOC(x, y) wip_realloc(x, y, NULL)
#define MSH_PLY_FREE(x) wip_free(x)
#define MSH_PLY_IMPLEMENTATION
#include "external/msh_ply.h"

wip_ply_t *wip_readModel(wip_ply_t *p, const char *file) {
	wip_debug(WIP_INFO, "%s: Loading model from %s...", __func__, file);

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

	msh_ply_desc_t formatUv = {
		.element_name = "vertex",
		.property_names = (const char*[]){"s", "t"},
		.num_properties = 2,
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

	int normal_c, uv_c, color_c;

	formatNormal.data = &p->normal;
	formatNormal.data_count = &normal_c;

	formatUv.data = &p->uv;
	formatUv.data_count = &uv_c;

	formatColor.data = &p->color;
	formatColor.data_count = &color_c;

	formatFace.data = &p->index;
	formatFace.data_count = &p->index_c;

	bool hasNormal = true;
	bool hasUv = true;
	bool hasColor = true;
	int r = 0;

	// TODO: do proper cleanup after failure

	msh_ply_t *ply_file = msh_ply_open(file, "r");
	if(!ply_file) {
		wip_log(WIP_ERROR, "%s: Couldn't open model %s", __func__, file);
		return NULL;
	}

	r = msh_ply_parse_header(ply_file);
	if(r) {
		wip_log(WIP_ERROR, "%s: Couldn't parse header: %s", __func__, msh_ply_error_msg(r));
		return NULL;
	}

	r = msh_ply_add_descriptor(ply_file, &formatVertex);
	if(r) {
		wip_log(WIP_ERROR, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));
		return NULL;
	}

	if(msh_ply_has_properties(ply_file, &formatNormal)) msh_ply_add_descriptor(ply_file, &formatNormal);
	else hasNormal = false;

	if(msh_ply_has_properties(ply_file, &formatUv)) msh_ply_add_descriptor(ply_file, &formatUv);
	else hasUv = false;

	if(msh_ply_has_properties(ply_file, &formatColor)) msh_ply_add_descriptor(ply_file, &formatColor);
	else hasColor = false;

	r = msh_ply_add_descriptor(ply_file, &formatFace);
	if(r) {
		wip_log(WIP_ERROR, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));
		return NULL;
	}

	r = msh_ply_read(ply_file);
	if(r) {
		wip_log(WIP_ERROR, "%s: Couldn't read model: %s", __func__, msh_ply_error_msg(r));
		return NULL;
	}

	wip_free(msh_ply_find_element(ply_file, "vertex")->data);
	wip_free(msh_ply_find_element(ply_file, "face")->data);
	msh_ply_close(ply_file);
	
	if(!hasNormal) { 
		wip_debug(WIP_WARN, "%s: Couldn't load vertex normals from model, may be missing.", __func__);
		*((void **)formatNormal.data) = wip_alloc((int)*formatVertex.data_count * 3 * sizeof(float));
	}
	if(!hasUv) {
		wip_debug(WIP_WARN, "%s: Couldn't load UVs from model, may be missing.", __func__);
		*((void **)formatUv.data) = wip_alloc((int)*formatVertex.data_count * 2 * sizeof(float));
	}
	if(!hasColor) {
		wip_debug(WIP_WARN, "%s: Couldn't load vertex colors from model, may be missing.", __func__);
		*((void **)formatColor.data) = wip_alloc((int)*formatVertex.data_count * 4 * sizeof(uint8_t));
	}

	for(int i = 0; i < (int)*formatVertex.data_count; ++i) {
		if(!hasNormal) {
			float *normalData = *(void **)formatNormal.data;
			memcpy(
				&normalData[3*i],
				(float []){0, 0, 0},
				3 * sizeof(float)
			);
		}
		if(!hasUv) {
			float *uvData = *(void **)formatUv.data;
			memcpy(
				&uvData[2*i],
				(float []){0, 0, 0},
				2 * sizeof(float)
			);
		}
		if(!hasColor) {
			uint8_t *colorData = *(void **)formatColor.data;
			memcpy(
				&colorData[4*i],
				(uint8_t []){255, 255, 255, 255},
				4 * sizeof(uint8_t)
			);
		}
	}

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return p;
}

wip_mdl_t *wip_prepModel(wip_mdl_t *m, wip_ply_t *p) {
	m->vertex_c = p->vertex_c;
	m->index_c = p->index_c;

	m->model = wip_alloc(p->vertex_c*sizeof(m->model[0]));
	for(int i = 0; i < p->vertex_c; ++i) {
		memcpy(m->model[i].vertex, &p->vertex[i*3], 3*sizeof(float));
		memcpy(m->model[i].normal, &p->normal[i*3], 3*sizeof(float));
		memcpy(m->model[i].uv, &p->uv[i*2], 2*sizeof(float));
		memcpy(m->model[i].color, &p->color[i*4], 4*sizeof(uint8_t));
	}

	m->index = wip_alloc(3*p->index_c*sizeof(uint32_t));
	memcpy(m->index, p->index, 3*p->index_c*sizeof(uint32_t));

	return m;
}

wip_glmdl_t *wip_loadModel(wip_glmdl_t *gm, wip_mdl_t *m) {
	gm->element_c = m->index_c * 3;

	glGenVertexArrays(1, &gm->vertex_a);
	glBindVertexArray(gm->vertex_a);

	glGenBuffers(2, gm->buffers);
	glBindBuffer(GL_ARRAY_BUFFER, gm->data_b);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gm->element_b);

	glBufferData(GL_ARRAY_BUFFER, m->vertex_c * sizeof(wip_vertex_t), m->model, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * m->index_c * sizeof(uint32_t), m->index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(wip_vertex_t), (void *)offsetof(wip_vertex_t, vertex));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(wip_vertex_t), (void *)offsetof(wip_vertex_t, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(wip_vertex_t), (void *)offsetof(wip_vertex_t, uv));
	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(wip_vertex_t), (void *)offsetof(wip_vertex_t, color));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return gm;
}

wip_glmdl_t *wip_openModel(char *name) {
	wip_ply_t ply;
	wip_mdl_t mdl;
	wip_glmdl_t *glmdl = wip_alloc(sizeof(wip_glmdl_t));

	// TODO: resource finding function
	const char *prefix = "./res/mdl/";
	const char *suffix = ".ply";
	char *filename = wip_alloc(strlen(prefix)+strlen(name)+strlen(suffix)+1);
	sprintf(filename, "%s%s%s", prefix, name, suffix);

	wip_readModel(&ply, filename);
	wip_free(filename);

	wip_prepModel(&mdl, &ply);
	wip_free(ply.vertex);
	wip_free(ply.index);
	wip_free(ply.color);
	wip_free(ply.normal);

	wip_loadModel(glmdl, &mdl);
	wip_free(mdl.model);
	wip_free(mdl.index);

	return glmdl;
}

