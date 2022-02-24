// WIP
// Copyright (c) 2021 Jovan Lanik

// Argument Parsing

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <getopt.h>
#endif

#include "wip_fn.h"

#define AUTHOR_EMAIL "jox969@gmail.com"

#define _STR(x) __STR(x)
#define __STR(x) #x
const char *wip_globalVersion ="WIP "_STR(WIP_NAME)" built with "_STR(WIP_WINDOW_BACKEND)", "__DATE__" at "__TIME__;
#undef __STR
#undef _STR
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

#ifdef _WIN32
char *wip_parseOptions(int argc, char *argv[]) { if(argc-1) wip_log(WIP_WARN, "%s: Command-line arguments not supported under Windows.", __func__); return NULL; }
#else
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
#endif

