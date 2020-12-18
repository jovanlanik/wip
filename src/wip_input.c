//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Input Functions

#include <string.h>

#include "wip_fn.h"
#include "wip_types.h"
#include "wip_input.h"
#include "wip_conf.h"
#include "wip_game.h"

#define KEY(key, code) [WIP_##key] = { #key }
char wip_globalKeyName[][64] = {
	WIP_KEY_LIST
};
#undef KEY

#ifdef WIP_MOTION
extern wip_motion_t wip_globalMotion[WIP_MOTION_END];
extern wip_motion_t *wip_globalKey[WIP_KEY_END];
extern char wip_globalMotionName[WIP_MOTION_END][64];

int wip_readMotion(enum wip_motion m) {
	wip_motion_t *motion = &wip_globalMotion[m];
	int ret = motion->state;
	if(motion->type == WIP_ONCE_PRESS || motion->type == WIP_ONCE_RELEASE)
		motion->state = 0;
	return ret;
}

void wip_prepMotion(wip_key_t key) {
	wip_motion_t *motion = wip_globalKey[key.key];
	if(motion) {
		wip_debug(WIP_INFO, "%s: Motion %s from key %s", __func__,
			wip_globalMotionName[motion->motion],
			wip_globalKeyName[key.key]
			);
		switch(motion->type) {
			case WIP_HOLD:
				if(key.action == WIP_RELEASE) motion->state = 0;
			case WIP_ONCE_PRESS:
				if(key.action == WIP_PRESS) motion->state = 1;
				break;
			case WIP_ONCE_RELEASE:
				if(key.action == WIP_RELEASE) motion->state = 1;
				break;
		}
	}
	return;
}

void wip_bindMotion(enum wip_motion m, enum wip_key k) {
	wip_motion_t *motion = &wip_globalMotion[m];
	wip_globalKey[motion->key] = NULL;
	wip_globalKey[k] = motion;
	motion->key = k;
	return;
}

void wip_loadBindings(void) {
	char *c = wip_alloc(64 + 5);
	for(int i = 0; i < WIP_MOTION_END; ++i ) {
		strcpy(c, "keys.");
		strcat(c, wip_globalMotionName[i]);
		if(wip_findConfStr(c))
			wip_bindMotion(i, wip_findKey(wip_getConfStr(c)));
	}
}
#endif

struct WIP_FIFO_T(WIP_KEY_BUFFER, wip_key_t) wip_key;

enum wip_key wip_findKey(const char *name) {
	for(int i = 0; i < WIP_KEY_END; ++i)
		if(strcmp(name, wip_globalKeyName[i]) == 0) return i;
	return 0;
}

int wip_writeKey(wip_key_t key) {
	if(wip_key.head + 1 == wip_key.tail) return 0;
	wip_key.head = (wip_key.head + 1) % WIP_KEY_BUFFER;
	wip_key.buffer[wip_key.head] = key;
	return 1;
}

wip_key_t wip_readKey(void) {
	if(wip_key.head == wip_key.tail)
		return (wip_key_t){ WIP_NONE, WIP_UNKNOWN };
	wip_key.tail = (wip_key.tail + 1) % WIP_KEY_BUFFER;
	return wip_key.buffer[wip_key.tail];
}

