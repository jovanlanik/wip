// Flipdot demo
// Copyright (c) 2023 Jovan Lanik

// Keymaps, Motions and other input

#pragma once

#define WIP_KEY_BUFFER 8
#define WIP_MOTION

#define MOTION_LIST \
	MOTION(WIP_ONCE, UP, WIP_UP) \
	MOTION(WIP_ONCE, DOWN, WIP_DOWN) \
	MOTION(WIP_ONCE, LEFT, WIP_LEFT) \
	MOTION(WIP_ONCE, RIGHT, WIP_RIGHT) \
	MOTION(WIP_ONCE, EVENT, WIP_SPACE) \
	MOTION(WIP_ONCE, QUIT, WIP_ALPHA-'a'+'q')

#define MOTION(type, motion, key) motion,

enum wip_motion {
	MOTION_LIST
	WIP_MOTION_END
};
#undef MOTION

