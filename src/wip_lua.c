// WIP
// Copyright (c) 2024 Jovan Lanik

// Lua embedding

#include "wip_fn.h"
#include "wip_lua.h"

lua_State *wip_luaInit(void) {
	wip_debug(WIP_INFO, "%s: Initializing Lua...", __func__);

	// TODO: own alloc func?
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return L;
}

