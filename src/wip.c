//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Main Source

#include <pthread.h>

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_window.h"
#include "wip_game.h"
#include "wip_input.h"

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

int main(int argc, char *argv[]) {
#define STR(x) #x
#define NAME(x) STR(x)
	wip_debug(WIP_INFO, "WIP "NAME(WIP_NAME)" built %s %s", __DATE__, __TIME__);
#undef STR
#undef NAME

	pthread_t logic;
	pthread_t render;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	wip_initConf();
	wip_loadBindings();

	wip_initWindow();
	pthread_mutex_init(&wip_globalWindow_m, NULL);

	pthread_create(&logic, &attr, wip_logicThread, NULL);
	pthread_create(&render, &attr, wip_renderThread, NULL);
	pthread_attr_destroy(&attr);

	//double startTime;

	while(!wip_globalWindow.close) {
		//startTime = wip_timeWindow();
		pthread_mutex_lock(&wip_globalWindow_m);
		wip_pollWindow();
		pthread_mutex_unlock(&wip_globalWindow_m);
		//while(wip_timeWindow() - startTime < 1.0/WIP_TICKRATE && !wip_globalWindow.close)
		wip_sleep(0.0001);
	}

	pthread_join(logic, NULL);
	pthread_join(render, NULL);

	wip_termWindow();
	pthread_mutex_destroy(&wip_globalWindow_m);

	wip_termConf();

	return 0;
}

