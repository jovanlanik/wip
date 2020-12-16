//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Input Functions

#pragma once

#include "wip_keys.h"

enum wip_key {
	WIP_UNKNOWN = 0,
	WIP_KP_NUM_LOCK = 1,
	WIP_KP = 2,
	WIP_KP_ENTER = 4,
	WIP_BS = 8, WIP_BACKSPACE = 8,
	WIP_TAB = 9,
	WIP_PRINT = 10, WIP_PRINT_SCREEN = 10,
	WIP_SCROLL = 11, WIP_SCROLL_LOCK = 11,
	WIP_PAUSE = 12,
	WIP_ENTER = 13, WIP_RETURN = 13,
	WIP_L_CTRL,
	WIP_L_SHIFT,
	WIP_L_GUI,
	WIP_L_ALT,
	WIP_R_CTRL,
	WIP_R_SHIFT,
	WIP_R_GUI,
	WIP_R_ALT,
	WIP_INSERT,
	WIP_HOME,
	WIP_END,
	WIP_PAGEUP,
	WIP_PAGEDOWN,
	WIP_ESC = 27, WIP_ESCAPE = 27,
	WIP_R = 28, WIP_RIGHT = 28,
	WIP_L = 29, WIP_LEFT = 29,
	WIP_D = 30, WIP_DOWN = 30,
	WIP_U = 31, WIP_UP = 31,
	WIP_SPACE = ' ', WIP_SPACEBAR = ' ',
	WIP_NUM = '0', WIP_NUMBER = '0',
	WIP_CAPS = 65, WIP_CAPS_LOCK = 65,
	WIP_F = 'A', WIP_FUNCTION = 'A',
	WIP_MENU = 90,
	WIP_ALPHA = 'a',
	WIP_DEL = 127, WIP_DELETE = 127,
	WIP_KP_NUM = 128,
	WIP_KEY_END
};

enum wip_key_action {
	WIP_NONE,
	WIP_PRESS,
	WIP_RELEASE
};

typedef struct {
	unsigned int action : 2;
	unsigned int key : 8;
} wip_key_t;

#ifdef WIP_MOTION
enum wip_motion_type {
	WIP_ONCE, WIP_ONCE_PRESS = 0,
	WIP_ONCE_RELEASE,
	WIP_HOLD
};

typedef struct {
	unsigned int state : 1;
	const unsigned int type: 2;
	const unsigned int motion : WIP_MOTION_END/2;
	unsigned int key : 8;
} wip_motion_t;

int wip_readMotion(enum wip_motion m);
void wip_prepMotion(wip_key_t key);
void wip_bindMotion(enum wip_motion m, enum wip_key k);
#endif

int wip_writeKey(wip_key_t k);
wip_key_t wip_readKey(void);

