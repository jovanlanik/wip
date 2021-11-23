// WIP
// Copyright (c) 2020 Jovan Lanik

// Keymaps, Motions and other input

#pragma once

#define WIP_KEY_BUFFER 16
#define WIP_MOTION

/*		Type		Name		Default Key	*/
#define MOTION_LIST \
	MOTION(WIP_HOLD,	MOVE_FORWARD,	WIP_UP), \
	MOTION(WIP_HOLD,	MOVE_BACK,	WIP_DOWN), \
	MOTION(WIP_HOLD,	STRAFE_LEFT,	WIP_LEFT), \
	MOTION(WIP_HOLD,	STRAFE_RIGHT,	WIP_RIGHT), \
	MOTION(WIP_HOLD,	CROUCH,		WIP_L_CTRL), \
	MOTION(WIP_ONCE,	JUMP,		WIP_SPACE), \
	MOTION(WIP_ONCE,	USE,		'e')

#define MOTION(type, motion, key) motion
enum wip_motion {
	MOTION_LIST,
	WIP_MOTION_END
};
#undef MOTION

