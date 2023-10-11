// Flipdot demo
// Copyright (c) 2020 Jovan Lanik

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
	if(wip_getConfBool("video.offscreen"))
		wip_initOffscreen(wip_getConfInt("video.width"), wip_getConfInt("video.height"));
	else
		wip_initWindow();
	wip_initGl();
	wip_gameLoop();
	wip_termWindow();
	wip_termConf();
}

