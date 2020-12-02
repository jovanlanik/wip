//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Configuration Functions

#pragma once

char *wip_getConfPath(void);
void wip_initConf(void);
void wip_termConf(void);

#ifndef WIP_DEFINE_CONF_TYPE
	#define WIP_DEFINE_CONF_TYPE(type, name, lib, def) \
	type wip_getConf##name(const char *p); \
	int wip_setConf##name(const char *p, type val);
#endif

WIP_DEFINE_CONF_TYPE(const char *, Str, string, NULL)
WIP_DEFINE_CONF_TYPE(int, Int, int, 0)
WIP_DEFINE_CONF_TYPE(double, Float, float, 0)
WIP_DEFINE_CONF_TYPE(int, Bool, bool, 0)

