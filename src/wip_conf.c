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
#include "wip_conf.h"

extern const char *wip_defaultConf;

config_t wip_globalConf;

// TODO: Add unknown setting to config if not found.
#define CONF_TYPE(type, name, lib, mac, def) \
int wip_findConf##name(const char*path) { \
	config_setting_t *s = config_lookup(&wip_globalConf, path); \
	if(s && config_setting_type(s) == mac) return 1; \
	return 0; \
} \
type wip_getConf##name(const char *path) { \
	type x = def; \
	if(!config_lookup_##lib(&wip_globalConf, path, &x)) \
		wip_log(WIP_WARN, "%s: Can't find '%s' in config.", __func__, path); \
	return x; \
} \
int wip_setConf##name(const char *path, type val) { \
	config_setting_t *setting = config_lookup(&wip_globalConf, path); \
	if(!setting || !config_setting_set_##lib(setting, val)) { \
		wip_log(WIP_WARN, "%s: Can't set '%s': Not in config.", __func__, path); \
		return 0; \
	} \
	return 1; \
}
WIP_CONF_TYPE_LIST
#undef CONF_TYPE

char *wip_getConfPath(void) {
	char *config = getenv("XDG_CONFIG_HOME");
	if(config != NULL && *config != '\0') {
		char *c = wip_alloc(strlen(config)+1);
		if(!c) return NULL;
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
			char *c = wip_alloc(strlen(pwd->pw_dir)+1);
			if(!c) return NULL;
			strcpy(c, pwd->pw_dir);
			config = c;
		}
		else {
			char *c = wip_alloc(strlen(config)+1);
			if(!c) return NULL;
			strcpy(c, config);
			config = c;
		}
		char dirname[] = "/.config";
		int ret;
		char *c = wip_realloc(config, strlen(config)+sizeof(dirname), &ret);
		if(!ret) return NULL;
		strcat(c, dirname);
		config = c;
	}
#define STR(x) #x
#define NAME(x) STR(x)
	char filename[] = "/wip/"NAME(WIP_NAME)".conf";
#undef STR
#undef NAME
	int ret;
	char *c = wip_realloc(config, strlen(config)+sizeof(filename), &ret);
	if(!ret) return NULL;
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
		ret = config_read_string(&wip_globalConf, wip_defaultConf);
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

