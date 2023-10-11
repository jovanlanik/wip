// WIP
// Copyright (c) 2023 Jovan Lanik

// OSMesa Window Functions

#define _POSIX_C_SOURCE 199309L

#include <time.h>
#include <unistd.h>
#include <GL/osmesa.h>

#include "wip_fn.h"
#include "wip_window.h"
#include "wip_conf.h"

#ifndef _POSIX_MONOTONIC_CLOCK
	#error Monotonic clock not supported.
#endif

wip_window_t wip_globalWindow;
static void *buffer = NULL;
static double startTime;

void wip_initWindow(void) {
	wip_log(WIP_WARN, "%s: Only offscreen rendering supported in backend.", __func__);
	wip_initOffscreen(wip_getConfInt("video.width"), wip_getConfInt("video.height"));
}

void wip_initOffscreen(int width, int height) {
	wip_debug(WIP_INFO, "%s: Initializing offscreen window...", __func__);

	const int attr[] = {
		OSMESA_FORMAT, OSMESA_RGBA,
		OSMESA_DEPTH_BITS, 0,
		OSMESA_STENCIL_BITS, 0,
		OSMESA_ACCUM_BITS, 0,
		OSMESA_PROFILE, OSMESA_CORE_PROFILE,
		OSMESA_CONTEXT_MAJOR_VERSION, 3,
		OSMESA_CONTEXT_MINOR_VERSION, 3,
		0, 0
	};

	wip_globalWindow.handle = OSMesaCreateContextAttribs(attr, NULL);
	if(!wip_globalWindow.handle)
		wip_log(WIP_FATAL, "%s: Couldn't create OSMesa context.", __func__);

	buffer = wip_alloc(width*height*4*sizeof(GLuint));
	if(!buffer)
		wip_log(WIP_FATAL, "%s: Couldn't allocate image buffer.", __func__);

	if(!OSMesaMakeCurrent(wip_globalWindow.handle, buffer, GL_UNSIGNED_BYTE, width, height))
		wip_log(WIP_FATAL, "%s: Couldn't bind OSMesa context.", __func__);

	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	startTime = time.tv_sec + time.tv_nsec * 1e-9;

	return;
}

void wip_swapWindow(void) { return; }
void wip_pollWindow(void) { return; }

double wip_timeWindow(void) {
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_sec + time.tv_nsec * 1e-9 - startTime;
}

void wip_termWindow(void) {
	wip_debug(WIP_INFO, "%s: Terminating window...", __func__);
	OSMesaDestroyContext(wip_globalWindow.handle);
	wip_debug(WIP_INFO, "%s: Done.", __func__);
}

