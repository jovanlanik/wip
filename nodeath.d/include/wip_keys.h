//
// WIP
// Copyright (c) 2021 Jovan Lanik
//

// KeyMaps, Motions and other input

#define WIP_KEY_BUFFER 16
#define WIP_MOTION

/*		Type		Name		Default Key	*/
#define MOTION_LIST \
	MOTION(WIP_HOLD,	MOVE_UP,	WIP_UP), \
	MOTION(WIP_HOLD,	MOVE_DOWN,	WIP_DOWN), \
	MOTION(WIP_HOLD,	MOVE_LEFT,	WIP_LEFT), \
	MOTION(WIP_HOLD,	MOVE_RIGHT,	WIP_RIGHT), \
	MOTION(WIP_ONCE,	SHOOT,		WIP_SPACE)

#define MOTION(type, motion, key) motion
enum wip_motion {
	MOTION_LIST,
	WIP_MOTION_END
};
#undef MOTION

