// WIP
// Copyright (c) 2020 Jovan Lanik

// Configuration Functions

#pragma once

char *wip_getConfPath(void);
void wip_initConf(void);
void wip_termConf(void);

#define WIP_CONF_TYPE_LIST \
	CONF_TYPE(const char *, Str, string, CONFIG_TYPE_STRING, NULL) \
	CONF_TYPE(int, Int, int, CONFIG_TYPE_INT,0) \
	CONF_TYPE(double, Float, float, CONFIG_TYPE_FLOAT, 0.0f) \
	CONF_TYPE(int, Bool, bool, CONFIG_TYPE_BOOL, 0)

#define CONF_TYPE(type, name, lib, mac, def) \
int wip_findConf##name(const char *p); \
type wip_getConf##name(const char *p); \
int wip_setConf##name(const char *p, type val);
WIP_CONF_TYPE_LIST
#undef CONF_TYPE

