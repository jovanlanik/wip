// WIP
// Copyright (c) 2020 Jovan Lanik

// Configuration Functions

#pragma once

#include <stdbool.h>

char *wip_getConfPath(void);
void wip_initConf(void);
void wip_termConf(void);

#define WIP_CONF_TYPE_LIST \
	CONF_TYPE(const char *, const char *, Str, string, CONFIG_TYPE_STRING, NULL) \
	CONF_TYPE(int, int,Int, int, CONFIG_TYPE_INT,0) \
	CONF_TYPE(double, double,Float, float, CONFIG_TYPE_FLOAT, 0.0f) \
	CONF_TYPE(bool, int, Bool, bool, CONFIG_TYPE_BOOL, 0)

#define CONF_TYPE(ret_type, lib_type,name, lib, mac, def) \
bool wip_findConf##name(const char *p); \
ret_type wip_getConf##name(const char *p); \
bool wip_setConf##name(const char *p, ret_type val);

WIP_CONF_TYPE_LIST
#undef CONF_TYPE

