// WIP
// Copyright (c) 2021 Jovan Lanik

// Argument Parsing

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define AUTHOR_EMAIL "jox969@gmail.com"
#define VERSION "0.0"

FILE *arg_output = NULL;

void showUsage(char *name) {
	printf("Usage: %s [<options>]\n", name);
	return;
}

void showHelp(char *name) {
	showUsage(name);
	puts(
		"C language Basic Template generator\n\n"
		"\t-h, --help\t\tdisplay this help and exit\n"
		"\t-V, --version\t\tdisplay version information and exit\n"
		"\t-o, --output=<file>\toutput to file instead of stdout\n"
		"\nReport bugs to <"AUTHOR_EMAIL">."
	);
	return;
}

char *parseOptions(int argc, char *argv[]) {
	char opt = 0;
	static struct option options[] = {
		{ .name = "help", .val = 'h' },
		{ .name = "version", .val = 'V' },
		{ .name = "output", .val = 'o', .has_arg = required_argument },
		{ 0, 0, 0, 0 }
	};
	while((opt = getopt_long(argc, argv, "-hVo:", options, NULL)) != -1) {
		switch(opt) {
			case 1:
				if(strcmp(argv[optind-1], "help") != 0)
					return strdup(argv[optind-1]);
			case 'h':
				showHelp(argv[0]);
				exit(EXIT_SUCCESS);
			case 'V':
				printf("%s "VERSION"\n", argv[0]);
				exit(EXIT_SUCCESS);
			case 'o':
				if((arg_output =  fopen(optarg, "w")) == NULL)
					exit(EXIT_FAILURE);
				break;
			case '?':
				exit(EXIT_FAILURE);
		}
	}
	if(arg_output == NULL) arg_output = stdout;
	return NULL;
}

