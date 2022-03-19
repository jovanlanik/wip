// WIP
// Copyright (c) 2020 Jovan Lanik

// Functions

#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "wip_fn.h"

#ifndef NDEBUG
void *_wip_alloc(size_t size, const char *caller)
{
	//wip_debug(WIP_INFO, "%s: allocating %zu bytes from %s", __func__, size, caller);
	void *o = malloc(size);
	if(!o) wip_debug(WIP_ERROR, "%s: failed allocation from %s", __func__, caller);
	//else wip_debug(WIP_INFO, "%s: allocated at %p", __func__, o);
	return o;
}
#endif

#ifdef NDEBUG
void *wip_realloc(void *pointer, size_t size, int *ret)
#else
void *_wip_realloc(void *pointer, size_t size, int *ret, const char *caller)
#endif
{
	//wip_debug(WIP_INFO, "%s: reallocating %p to %zu bytes from %s", __func__, pointer, size, caller);
	void *o = realloc(pointer, size);
	if(!o) {
		if(ret) *ret = 0;
		wip_debug(WIP_ERROR, "%s: failed reallocation from %s, returning original", __func__, caller);
		return pointer;
	}
	if(ret) *ret = 1;
	//wip_debug(WIP_INFO, "%s: reallocated to %p", __func__, o);
	return o;
}

#ifndef NDEBUG
void _wip_free(void *pointer, const char *caller)
{
	//wip_debug(WIP_INFO, "%s: freeing %p from %s", __func__, pointer, caller);
	free(pointer);
	return;
}
#endif

int wip_atoi(char *s, int *i) {
	char *end = NULL;
	errno = 0;
	*i = strtol(s, &end, 10);
	int r = errno;
	if(r) wip_log(WIP_ERROR, "%s: %s", __func__, strerror(r));
	if(s == end) r = 1;
	return r;
}

int wip_atoui(char *s, unsigned int *i) {
	char *end = NULL;
	errno = 0;
	*i = strtoul(s, &end, 10);
	int r = errno;
	if(r) wip_debug(WIP_ERROR, "%s: %s", __func__, strerror(r));
	if(s == end) r = 1;
	return r;
}

void wip_log(enum wip_logType type, const char *message, ...) {
	FILE *out = type < WIP_WARN ? stdout : stderr;
	va_list args;
	va_start(args, message);
	vfprintf(out, message, args);
	fputc('\n', out);
	va_end(args);
	if(type == WIP_FATAL) exit(1);
	return;
}

FILE *wip_openFile(const char *name) {
	FILE *file = fopen(name, "r+");
	if(!file) wip_log(WIP_ERROR, "%s: Couldn't open %s: %s", __func__, name, strerror(errno));
	return file;
}

char *wip_readFile(FILE* file) {
	if(!file) return NULL;
	fseek(file, 0, SEEK_END);
	long int size = ftell(file);
	rewind(file);
	char *buff = wip_alloc(size+1);
	fread(buff, 1, size, file);
	buff[size] = '\0';
	return buff;
}

/*
void wip_sleep(double seconds) {
	double intpart;
	struct timespec time;
	time.tv_nsec = modf(seconds, &intpart) * 1000000000;
	time.tv_sec = intpart;
	//wip_debug(WIP_INFO, "%s: Slepping for %d seconds and %ld nanoseconds.",
	//	__func__, time.tv_sec, time.tv_nsec);
	if(nanosleep(&time, NULL)) wip_log(WIP_ERROR, "%s: interupted.", __func__);
	return;
}

void *timeoutFunc(void *arg) {
	wip_timeout_t *timeout = (wip_timeout_t *)arg;

	*timeout->done = 0;
	wip_sleep(timeout->time);
	timeout->func(timeout->arg);

	*timeout->done = 1;
	wip_free(timeout);
	pthread_exit(0);
}

int *wip_setTimeout(void *(*func)(void *), void *arg, double time) {
	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	wip_timeout_t *timeout = wip_alloc(sizeof(wip_timeout_t));
	timeout->done = wip_alloc(sizeof(int));
	timeout->time = time;
	timeout->func = func;
	timeout->arg = arg;

	pthread_create(&thread, &attr, timeoutFunc, timeout);
	pthread_attr_destroy(&attr);
	return timeout->done;
}
*/

