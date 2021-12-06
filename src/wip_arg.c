// WIP
// Copyright (c) 2021 Jovan Lanik

// Argument Parsing

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "wip_fn.h"

#define AUTHOR_EMAIL "jox969@gmail.com"

#define STR(x) #x
#define NAME(x) STR(x)
const char *wip_globalVersion ="WIP "NAME(WIP_NAME)" built with "NAME(WIP_WINDOW_BACKEND)", "__DATE__" at "__TIME__;
#undef STR
#undef NAME
const char *optstr = "-hVu";

void wip_showUsage(char *name) {
	wip_log(WIP_INFO, "Usage: %s %s", name, optstr);
	return;
}

void wip_showHelp(char *name) {
	wip_print(wip_globalVersion);
	wip_showUsage(name);
	wip_log(
		WIP_INFO,
		"\n\t-h, --help\t\tdisplay this help and exit\n"
		"\t-V, --version\t\tdisplay version information and exit\n"
		"\t-u, --usage\t\tdisplay usage and exit\n"
		"\nReport bugs to <%s>.",
		AUTHOR_EMAIL
	);
	return;
}

char *wip_parseOptions(int argc, char *argv[]) {
	char opt = 0;
	static struct option options[] = {
		{ .name = "help", .val = 'h' },
		{ .name = "version", .val = 'V' },
		{ .name = "usage", .val = 'u' },
		{ 0, 0, 0, 0 }
	};
	while((opt = getopt_long(argc, argv, optstr, options, NULL)) != -1) {
		switch(opt) {
			case 1:
				if(strcmp(argv[optind-1], "help") != 0)
					return strdup(argv[optind-1]);
			case 'h':
				wip_showHelp(argv[0]);
				exit(EXIT_SUCCESS);
			case 'V':
				wip_print(wip_globalVersion);
				exit(EXIT_SUCCESS);
			case 'u':
				wip_showUsage(argv[0]);
				exit(EXIT_SUCCESS);
			case '?':
				exit(EXIT_FAILURE);
		}
	}
	return NULL;
}

