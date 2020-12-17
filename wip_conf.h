//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Configuration Functions

#pragma once

char *wip_getConfPath(void);
void wip_initConf(void);
void wip_termConf(void);

#define WIP_CONF_TYPE_LIST \
	CONF_TYPE(const char *, Str, string, NULL) \
	CONF_TYPE(int, Int, int, 0) \
	CONF_TYPE(double, Float, float, 0.0) \
	CONF_TYPE(int, Bool, bool, 0.0f)

#define CONF_TYPE(type, name, lib, def) \
type wip_getConf##name(const char *p); \
int wip_setConf##name(const char *p, type val);
WIP_CONF_TYPE_LIST
#undef CONF_TYPE

