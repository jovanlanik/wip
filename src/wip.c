// WIP
// Copyright (c) 2020 Jovan Lanik

// WIP Source

#include "wip_fn.h"
#include "wip_arg.h"
#include "wip_conf.h"
#include "wip_window.h"
#include "wip_gl.h"
#include "wip_game.h"
#include "wip_input.h"

extern wip_window_t wip_globalWindow;

int main(int argc, char *argv[]) {
	wip_parseOptions(argc, argv);
	wip_initConf();
	wip_loadBindings();
	wip_initWindow();
	wip_glInit();
	wip_renderThread();
	wip_termWindow();
	wip_termConf();
	return 0;
}

