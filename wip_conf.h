//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Configuration Functions

char *wip_getConfPath(void);
void wip_initConf(void);
void wip_termConf(void);

#ifndef WIP_DEFINE_GET_CONF
	#define WIP_DEFINE_GET_CONF(type, name, lib, def) type wip_getConf##name(const char *p);
#endif

WIP_DEFINE_GET_CONF(const char *, Str, string, NULL)
WIP_DEFINE_GET_CONF(int, Int, int, 0)
WIP_DEFINE_GET_CONF(double, Float, float, 0)
WIP_DEFINE_GET_CONF(int, Bool, bool, 0)

