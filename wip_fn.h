//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Functions

#pragma once

#include <stdio.h>

#ifdef NDEBUG
	#define wip_debug(...) /* nothing */
	void *wip_alloc(size_t);
	void *wip_realloc(void *, size_t, int*);
	void wip_free(void *);
#else
	#define wip_debug(...) wip_log(__VA_ARGS__)
	#define wip_alloc(s) _wip_alloc(s, __func__)
	#define wip_realloc(p, s, r) _wip_realloc(p, s, r, __func__)
	#define wip_free(p) _wip_free(p, __func__)
	void *_wip_alloc(size_t, const char *);
	void *_wip_realloc(void *, size_t, int *, const char *);
	void _wip_free(void *, const char *);
#endif

#define WIP_ALLOCTYPE(t) wip_alloc(sizeof(t))
#define wip_print(s) wip_log(WIP_INFO, "%s", s);

enum wip_logType {
	WIP_INFO,
	WIP_WARN,
	WIP_ERROR,
	WIP_FATAL
};

typedef struct {
	int *done;
	double time;
	void *(*func)(void *);
	void *arg;
} wip_timeout_t;

void wip_log(enum wip_logType, const char *m, ...);
void wip_sleep(double seconds);
FILE *wip_openFile(const char *n);
//char *wip_readFile(void *f);
int *wip_setTimeout(void *(*func)(void *), void *arg, double time);

