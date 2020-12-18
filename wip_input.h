//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Input Functions

#pragma once

#include "wip_keys.h"

#define WIP_KEY_LIST \
	KEY(UNKNOWN,		= 0), \
	KEY(KP_NUM_LOCK,	= 1), \
	KEY(KP,			= 2), \
	KEY(KP_ENTER,		= 4), \
	KEY(BACKSPACE,		= 8), \
	KEY(TAB,		= 9), \
	KEY(PRINT_SCREEN,	= 10), \
	KEY(SCROLL_LOCK,	= 11), \
	KEY(PAUSE,		= 12), \
	KEY(ENTER,		= 13), \
	KEY(L_CTRL,		), \
	KEY(L_SHIFT,		), \
	KEY(L_GUI,		), \
	KEY(L_ALT,		), \
	KEY(R_CTRL,		), \
	KEY(R_SHIFT,		), \
	KEY(R_GUI,		), \
	KEY(R_ALT,		), \
	KEY(INSERT,		), \
	KEY(HOME,		), \
	KEY(END,		), \
	KEY(PAGEUP,		), \
	KEY(PAGEDOWN,		), \
	KEY(ESCAPE,		= 27), \
	KEY(RIGHT,		= 28), \
	KEY(LEFT,		= 29), \
	KEY(DOWN,		= 30), \
	KEY(UP,			= 31), \
	KEY(SPACE,		= ' '), \
	KEY(CAPS,		= 65), \
	KEY(MENU,		= 90), \
	KEY(DELETE,		= 127)

#define WIP_KEY_ALT_LIST \
	KEY(BS,			= 8), \
	KEY(PRINT,		= 10), \
	KEY(SCROLL,		= 11), \
	KEY(RETURN,		= 13), \
	KEY(ESC,		= 27), \
	KEY(R,			= 28), \
	KEY(L,			= 29), \
	KEY(D,			= 30), \
	KEY(U,			= 31), \
	KEY(SPACEBAR,		= ' '), \
	KEY(NUM,		= '0'), \
	KEY(NUMBER,		= '0'), \
	KEY(CAPS_LOCK,		= 65), \
	KEY(F,			= 'A'), \
	KEY(FUNCTION,		= 'A'), \
	KEY(ALPHA,		='a'), \
	KEY(DEL,		= 127), \
	KEY(KP_NUM,		= 128)

#define KEY(key, code) WIP_##key code
enum wip_key {
	WIP_KEY_LIST,
	WIP_KEY_ALT_LIST,
	WIP_KEY_END
};
#undef KEY

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

// TODO: wip_findMotion

int wip_readMotion(enum wip_motion m);
void wip_prepMotion(wip_key_t key);
void wip_bindMotion(enum wip_motion m, enum wip_key k);
void wip_loadBindings(void);
#endif

enum wip_key wip_findKey(const char *name);
int wip_writeKey(wip_key_t k);
wip_key_t wip_readKey(void);

