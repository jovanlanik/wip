// WIP
// Copyright (c) 2020 Jovan Lanik

// Window Functions

#pragma once

typedef struct {
	int close;
	void *handle;
} wip_window_t;

void wip_initWindow(void);
void wip_initOffscreen(int width, int height);
void wip_swapWindow(void);
void wip_pollWindow(void);
double wip_timeWindow(void);
void wip_termWindow(void);

