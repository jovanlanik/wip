//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Model Functions

#pragma once

typedef struct {
	float *vertex;
	float *normal;
	uint8_t *color;
	uint32_t *index;
	int vertex_c;
	int index_c;
} wip_ply_t;

typedef struct {
	void *model;
	uint32_t *index;
	int vertex_c;
	int index_c;
} wip_mdl_t;

wip_ply_t *wip_readModel(wip_ply_t *p, char *file);
wip_mdl_t *wip_prepModel(wip_mdl_t *m, wip_ply_t *p);

