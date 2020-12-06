//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Configuration Functions

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <libconfig.h>

#include "wip_fn.h"

#include "baked/config.h"

config_t wip_globalConf;

// TODO: Add unknown setting to config if not found.
#define WIP_DEFINE_CONF_TYPE(type, name, lib, def) \
type wip_getConf##name(const char *path) { \
	type x = def; \
	if(!config_lookup_##lib(&wip_globalConf, path, &x)) \
		wip_log(WIP_ERROR, "%s: Can't find '%s' in config.", __func__, path); \
	return x; \
} \
int wip_setConf##name(const char *path, type val) { \
	config_setting_t *setting = config_lookup(&wip_globalConf, path); \
	if(!setting || !config_setting_set_##lib(setting, val)) { \
		wip_log(WIP_ERROR, "%s: Can't set '%s': Not in config.", __func__, path); \
		return 0; \
	} \
	return 1; \
}

#include "wip_conf.h"

char *wip_getConfPath(void) {
	char *config = getenv("XDG_CONFIG_HOME");
	if(config != NULL && *config != '\0') {
		char *c = malloc(strlen(config)+1);
		if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
		strcpy(c, config);
		config = c;

	}
	else {
		config = getenv("HOME");
		if(config == NULL || *config == '\0') {
			errno = 0;
			struct passwd *pwd = getpwuid(getuid());
			if(!pwd) {
				wip_log(WIP_ERROR, "%s: Couldn't find user home: %s", __func__, strerror(errno));
				return NULL;
			}
			char *c = malloc(strlen(pwd->pw_dir)+1);
			if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
			strcpy(c, pwd->pw_dir);
			config = c;
		}
		else {
			char *c = malloc(strlen(config)+1);
			if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
			strcpy(c, config);
			config = c;
		}
		char dirname[] = "/.config";
		char *c = realloc(config, strlen(config)+sizeof(dirname));
		strcat(c, dirname);
		config = c;
	}
	char filename[] = "/wip/wip.conf";
	char *c = realloc(config, strlen(config)+sizeof(filename));
	if(!c) wip_log(WIP_FATAL, "%s: failed allocation", __func__);
	strcat(c, filename);
	config = c;
	return config;
}

void wip_initConf(void) {
	config_init(&wip_globalConf);
	char *confPath = wip_getConfPath();
	wip_debug(WIP_INFO, "%s: Loading config from %s...", __func__, confPath);
	FILE *confFile = wip_openFile(confPath);
	free(confPath);
	int ret;
	if(!confFile) {
		wip_log(WIP_WARN, "%s: No config file found. Using default.", __func__);
		ret = config_read_string(&wip_globalConf, wip_conf);
	}
	else ret = config_read(&wip_globalConf, confFile);
	if(!ret) {
		wip_log(
			WIP_FATAL, "%s: Config error:\n%s:%d - %s\n", __func__,
			config_error_file(&wip_globalConf),
			config_error_line(&wip_globalConf),
			config_error_text(&wip_globalConf)
		);
		config_destroy(&wip_globalConf);
		return;
	}
	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return;
}

void wip_termConf(void) {
	config_destroy(&wip_globalConf);
	return;
}

