// WIP
// Copyright (c) 2020 Jovan Lanik

// Input Functions

#pragma once

#include <stdbool.h>

#include "wip_keys.h"

#define WIP_KEY_LIST \
	KEY(UNKNOWN,		= 0) \
	KEY(KP_NUM_LOCK,	= 1) \
	KEY(KP,			= 2) \
	KEY(KP_ENTER,		= 4) \
	KEY(BACKSPACE,		= 8) \
	KEY(TAB,		= 9) \
	KEY(PRINT_SCREEN,	= 10) \
	KEY(SCROLL_LOCK,	= 11) \
	KEY(PAUSE,		= 12) \
	KEY(ENTER,		= 13) \
	KEY(L_CTRL,		) \
	KEY(L_SHIFT,		) \
	KEY(L_GUI,		) \
	KEY(L_ALT,		) \
	KEY(R_CTRL,		) \
	KEY(R_SHIFT,		) \
	KEY(R_GUI,		) \
	KEY(R_ALT,		) \
	KEY(INSERT,		) \
	KEY(HOME,		) \
	KEY(END,		) \
	KEY(PAGEUP,		) \
	KEY(PAGEDOWN,		) \
	KEY(ESCAPE,		= 27) \
	KEY(RIGHT,		= 28) \
	KEY(LEFT,		= 29) \
	KEY(DOWN,		= 30) \
	KEY(UP,			= 31) \
	KEY(SPACE,		= ' ') \
	KEY(CAPS_LOCK,		= 65) \
	KEY(MENU,		= 90) \
	KEY(DELETE,		= 127) \
	KEY(LOCKED,		= 255)

#define WIP_KEY_ALT_LIST \
	KEY(BS,			= WIP_BACKSPACE) \
	KEY(PRINT,		= WIP_PRINT_SCREEN) \
	KEY(SCROLL,		= WIP_SCROLL_LOCK) \
	KEY(RETURN,		= WIP_ENTER) \
	KEY(CTRL,		= WIP_L_CTRL) \
	KEY(SHIFT,		= WIP_L_SHIFT) \
	KEY(GUI,		= WIP_L_GUI) \
	KEY(ALT,		= WIP_L_ALT) \
	KEY(ESC,		= WIP_ESCAPE) \
	KEY(R,			= WIP_RIGHT) \
	KEY(L,			= WIP_LEFT) \
	KEY(D,			= WIP_DOWN) \
	KEY(U,			= WIP_UP) \
	KEY(SPACEBAR,		= WIP_SPACE) \
	KEY(NUM,		= '0') \
	KEY(NUMBER,		= WIP_NUM) \
	KEY(CAPS,		= WIP_CAPS_LOCK) \
	KEY(F,			= 'A') \
	KEY(FUNCTION,		= WIP_F) \
	KEY(ALPHA,		= 'a') \
	KEY(DEL,		= WIP_DELETE) \
	KEY(KP_NUM,		= 128) \
	KEY(KP_NUM_LAST,	= WIP_KP_NUM+9)

#define KEY(key, code) WIP_##key code,

enum wip_key {
	WIP_KEY_LIST
	WIP_KEY_ALT_LIST
	WIP_KEY_END,
};
#undef KEY

enum wip_key_action {
	WIP_NONE,
	WIP_PRESS,
	WIP_RELEASE,
};

typedef struct {
	unsigned int action : 2;
	enum wip_key key;
} wip_key_t;

#ifdef WIP_MOTION
enum wip_motion_type {
	WIP_ONCE,
	WIP_ONCE_PRESS = 0,
	WIP_ONCE_RELEASE,
	WIP_HOLD,
};

typedef struct {
	bool state : 1;
	const unsigned int type : 2;
	enum wip_motion motion;
	enum wip_key key;
} wip_motion_t;

void wip_clearMotions(void);
enum wip_motion wip_findMotion(const char *name);
bool wip_readMotion(enum wip_motion m);
bool wip_writeMotion(wip_key_t key);
void wip_bindMotion(enum wip_motion m, enum wip_key k);
void wip_loadBindings(void);
#endif

enum wip_key wip_findKey(const char *name);
bool wip_writeKey(wip_key_t k);
wip_key_t wip_readKey(void);

