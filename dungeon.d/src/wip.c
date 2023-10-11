// Dungeon
// Copyright (c) 2023 Jovan Lanik

// Source

#include "wip_arg.h"
#include "wip_conf.h"
#include "wip_window.h"
#include "wip_gl.h"
#include "wip_game.h"
#include "wip_input.h"

int main(int argc, char *argv[]) {
	wip_parseOptions(argc, argv);
	wip_initConf();
	wip_loadBindings();
	wip_initWindow();
	wip_initGl();
	wip_gameLoop();
	wip_termWindow();
	wip_termConf();
	return 0;
}

