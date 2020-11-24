//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Configuration Functions

char *wip_getConfPath(void);
void wip_initConf(void);
void wip_termConf(void);

#ifndef WIP_DEFINE_GET_CONF
	#define WIP_DEFINE_GET_CONF(type, name, lib) type wip_getConf##name(const char *p);
#endif

WIP_DEFINE_GET_CONF(const char *, Str, string)
WIP_DEFINE_GET_CONF(int, Int, int)
WIP_DEFINE_GET_CONF(double, Float, float)

