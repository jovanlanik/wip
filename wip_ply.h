//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// PLY model loading

#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int vertCount;
	float *vertData;
	float *vertNormal;
	unsigned char *vertColor;
	int indxCount;
	uint32_t *indxData;
} wip_modelPly;

wip_modelPly *wip_loadModelPly(wip_modelPly *m, char *file);

