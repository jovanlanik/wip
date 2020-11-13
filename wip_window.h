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

wip_window_t wip_globalWindow;
pthread_mutex_t wip_globalWindow_m;

void wip_initWindow(wip_window_t *w);
void wip_setWindow(wip_window_t *w);
void wip_swapWindow(wip_window_t *w);
void wip_pollWindow(wip_window_t *w);
void wip_termWindow(wip_window_t *w);

