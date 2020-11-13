//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Resource Management Functions

#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "wip_fn.h"
#include "wip_res.h"

void *wip_getResource(void *arg) {
	wip_res_t *request = (wip_res_t *)arg;
	switch(request->type) {
		case WIP_INTERNAL:
			break;
		case WIP_CONFIG:
			break;
		case WIP_FILE_HANDLE:
		case WIP_FILE_CONTENT:
			;
			void *file = wip_openFile(request->name, "r");
			if(request->type == WIP_FILE_HANDLE)
				request->data = (void *)file;
			else
				request->data = wip_readFile(file);
			break;
		default:
			wip_log(WIP_ERROR, "%s: Unknown resource type.", __func__);
	}
	return NULL;
}

