// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Keymaps, Motions and other input

#pragma once

#define WIP_KEY_BUFFER 16
#define WIP_MOTION

/*		Type		Name		Default Key	*/
#define MOTION_LIST \
	MOTION(WIP_ONCE,	UP,	WIP_UP), \
	MOTION(WIP_ONCE,        DOWN,     WIP_DOWN), \
	MOTION(WIP_ONCE,	LEFT,	WIP_LEFT), \
	MOTION(WIP_ONCE,	RIGHT,	WIP_RIGHT), \
	MOTION(WIP_ONCE,	USE,	WIP_ENTER), \
	MOTION(WIP_ONCE,	HELP,	WIP_ALPHA-'a'+'h'), \
	MOTION(WIP_ONCE,	ESC,	WIP_ESC)

#define MOTION(type, motion, key) motion
enum wip_motion {
	MOTION_LIST,
	WIP_MOTION_END
};
#undef MOTION

