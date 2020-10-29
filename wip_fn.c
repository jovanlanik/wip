//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Functions

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include "wip_fn.h"

#ifdef NDEBUG
	void *wip_alloc(size_t size)
#else
	void *_wip_alloc(size_t size, const char *caller)
#endif
{
	//wip_debug(WIP_INFO, "%s: allocating %zu bytes from %s", __func__, size, caller);
	void *o = malloc(size);
	if(!o) wip_debug(WIP_ERROR, "%s: failed allocation from %s", __func__, caller);
	//else wip_debug(WIP_INFO, "%s: allocated at %p", __func__, o);
	return o;
}

#ifdef NDEBUG
	void *wip_realloc(void *pointer, size_t size)
#else
	void *_wip_realloc(void *pointer, size_t size, const char *caller)
#endif
{
	//wip_debug(WIP_INFO, "%s: reallocating %p to %zu bytes from %s", __func__, pointer, size, caller);
	void *o = realloc(pointer, size);
	if(!o) {
		wip_debug(WIP_ERROR, "%s: failed reallocation from %s, returning original", __func__, caller);
		return pointer;
	}
	//wip_debug(WIP_INFO, "%s: reallocated to %p", __func__, o);
	return o;
}

#ifdef NDEBUG
	void wip_free(void *pointer)
#else
	void _wip_free(void *pointer, const char *caller)
#endif
{
	//wip_debug(WIP_INFO, "%s: freeing %p from %s", __func__, pointer, caller);
	free(pointer);
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

char *wip_getConf(void) {
	char *config = getenv("XDG_CONFIG_HOME");
	if(config != NULL && *config != '\0') {
		char *c = malloc(strlen(config)+1);
		if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
		strcpy(c, config);
		config = c;

	}
	else {
		config = getenv("HOME");
		if(config == NULL || *config == '\0') {
			errno = 0;
			struct passwd *pwd = getpwuid(getuid());
			if(!pwd) wip_log(WIP_FATAL, "%s: Couldn't find user home: %s", __func__, strerror(errno));
			char *c = malloc(strlen(pwd->pw_dir)+1);
			if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
			strcpy(c, pwd->pw_dir);
			config = c;
		}
		else {
			char *c = malloc(strlen(config)+1);
			if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
			strcpy(c, config);
			config = c;
		}
		char dirname[] = "/.config";
		char *c = realloc(config, strlen(config)+sizeof(dirname));
		strcat(c, dirname);
		config = c;
	}
	char filename[] = "/wip/wip.conf";
	char *c = realloc(config, strlen(config)+sizeof(filename));
	if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
	strcat(c, filename);
	config = c;
	return config;
}

