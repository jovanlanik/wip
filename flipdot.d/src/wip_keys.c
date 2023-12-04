// Flipdot demo
// Copyright (c) 2023 Jovan Lanik

// Keymaps, Motions and other input

#include "wip_input.h"

#define MOTION(t, m, k) [m] = { .state = 0, .type = t, .motion =  m, .key = k },

wip_motion_t wip_globalMotion[] = {
	MOTION_LIST
};
#undef MOTION

#define MOTION(type, motion, key) [motion] = { #motion },

char wip_globalMotionName[][64] = {
	MOTION_LIST
};
#undef MOTION

#define MOTION(type, motion, key) [key] = &wip_globalMotion[motion],

wip_motion_t *wip_globalKey[WIP_KEY_END] = {
	MOTION_LIST
};
#undef MOTION

