//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Resource Management Functions

#pragma once

enum wip_resType {
	WIP_INTERNAL,
	WIP_CONFIG,
	WIP_FILE_HANDLE,
	WIP_FILE_CONTENT,
	WIP_MODEL
};

typedef struct { 
	enum wip_resType type;
	char *name;
	void *data;
} wip_res_t;

void *wip_getResource(void *r);

