//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Functions

#pragma once

#ifdef NDEBUG
	#define wip_debug(...) ((void)0)
#else
	#define wip_debug(...) wip_log(__VA_ARGS__)
#endif

enum wip_logType {
	WIP_INFO,
	WIP_WARN,
	WIP_ERROR,
	WIP_FATAL
};

void wip_log(enum wip_logType, const char *message, ...);
char *wip_getConf(void);

