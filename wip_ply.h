//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// PLY model loading

#pragma once

typedef struct {
	int vertCount;
	float *vertData;
	float *vertNormal;
	unsigned char *vertColor;
	int indxCount;
	uint32_t *indxData;
} wip_modelPly;

wip_modelPly *wip_loadModelPly(wip_modelPly *m, char *file);

