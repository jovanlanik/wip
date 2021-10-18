// WIP
// Copyright (c) 2021 Jovan Lanik

// Game Logic

#include <stdlib.h>
#include <string.h>
//#include <stddef.h>
#include <math.h>
#include <pthread.h>

#include "wip_fn.h"
#include "wip_obj.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_game.h"
#include "wip_math.h"
#include "wip_conf.h"

#include "lib/linmath.h"

int wip_globalTicksPerSecond;

wip_scene_t wip_globalScene;

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

extern char wip_globalKeyName[][64];

void *wip_logicThread(void *arg) {
	wip_obj_t center, camera;

	wip_makeObject(&camera);
	camera.z = 6.5;
	camera.y = 0;

	wip_makeObject(&center);
	
	int sceneLength = 2;

	wip_globalScene.object = wip_alloc(sceneLength*sizeof(void *));
	wip_globalScene.object[0] = &center;
	wip_globalScene.object[1] = &camera;
	
	wip_globalScene.length = sceneLength;

	wip_globalTicksPerSecond = 0;
	double startTime, lastTime = wip_timeWindow();
	

	while(!wip_globalWindow.close) {
		startTime = wip_timeWindow();

		wip_globalTicksPerSecond++;
		if(startTime - lastTime >= 1.0) {
			lastTime = startTime;
			//wip_log(WIP_INFO, "Tickrate: %d\nTicktime: %f\n",
			//	wip_globalTicksPerSecond, 1000.0/wip_globalTicksPerSecond);
			wip_globalTicksPerSecond = 0;
		}

		wip_key_t key;
		while((key = wip_readKey()).action) {
			if(key.key == WIP_ESC || key.key == 'q') {
				pthread_mutex_lock(&wip_globalWindow_m);
				wip_globalWindow.close = 1;
				pthread_mutex_unlock(&wip_globalWindow_m);
			}
			wip_writeMotion(key);
		}

		while(wip_timeWindow() - startTime < 1.0/WIP_TICKRATE && !wip_globalWindow.close)
			wip_sleep(0.00001);
	}

	pthread_exit(NULL);
}

