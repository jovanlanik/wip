//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Window Functions

#pragma once

#include <pthread.h>

typedef struct {
	int close;
	void *handle;
} wip_window_t;

#ifndef WIP_MAIN
extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;
#endif

void wip_initWindow(wip_window_t *w);
void wip_setWindow(wip_window_t *w);
void wip_swapWindow(wip_window_t *w);
void wip_pollWindow(wip_window_t *w);
double wip_timeWindow(wip_window_t *w);
void wip_termWindow(wip_window_t *w);

