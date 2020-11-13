//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Main Source

#include <pthread.h>

#include "wip_fn.h"
#include "wip_res.h"
#include "wip_window.h"

void *wip_logicThread(void *arg);
void *wip_renderThread(void *arg);

int main(int argc, char *argv[]) {
	wip_debug(WIP_INFO, "WIP built %s %s", __DATE__, __TIME__);

	pthread_t logic;
	pthread_t render;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	wip_initWindow(&wip_globalWindow);
	pthread_mutex_init(&wip_globalWindow_m, NULL);

	pthread_create(&logic, &attr, wip_logicThread, NULL);
	pthread_create(&render, &attr, wip_renderThread, NULL);
	pthread_attr_destroy(&attr);

	while(!wip_globalWindow.close) {
		pthread_mutex_lock(&wip_globalWindow_m);
		wip_pollWindow(&wip_globalWindow);
		pthread_mutex_unlock(&wip_globalWindow_m);
	}

	pthread_join(logic, NULL);
	pthread_join(render, NULL);

	wip_termWindow(&wip_globalWindow);
	pthread_mutex_destroy(&wip_globalWindow_m);

	return 0;
}

