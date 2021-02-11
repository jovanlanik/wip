//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Game Logic

#include <stddef.h>
#include <math.h>
#include <pthread.h>

#include "wip_fn.h"
#include "wip_obj.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_game.h"

int wip_globalTicksPerSecond;

wip_scene_t wip_globalScene;
pthread_mutex_t wip_globalScene_m;

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

extern char wip_globalKeyName[][64];

wip_obj_t *getPointOnCircle(wip_obj_t *result, wip_obj_t *center, float angle, float radius) {
	*result = *center;
	result->x += radius * sin(angle);
	result->y += radius * cos(angle);
	return result;
}

void *wip_logicThread(void *arg) {
	wip_obj_t object[1];
	wip_obj_t light, eye, center;

	wip_makeObject(&eye);
	eye.y = -3;

	wip_makeObject(&center);
	getPointOnCircle(&center, &eye, eye.r.y, 1);

	wip_makeObject(&light);
	light.x = 2;
	light.y = 0;
	light.z = 1;

	wip_makeObject(&object[0]);

	wip_globalScene.object = wip_alloc(4*sizeof(void *));
	wip_globalScene.object[0] = &light;
	wip_globalScene.object[1] = &eye;
	wip_globalScene.object[2] = &center;
	wip_globalScene.object[3] = &object[0];
	wip_globalScene.length = 4;

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
		if(wip_readMotion(MOVE_FORWARD)) {
			eye.y -= eye.y - center.y;
			eye.x -= eye.x - center.x;
		}
		if(wip_readMotion(MOVE_BACK)) {
			eye.y += eye.y - center.y;
			eye.x += eye.x - center.x;
		}
		getPointOnCircle(&center, &eye, eye.r.y, 0.025);
		if(wip_readMotion(STRAFE_LEFT)) eye.r.y -= 0.02;
		if(wip_readMotion(STRAFE_RIGHT)) eye.r.y += 0.02;

		object[0].z = 0.5*sin(startTime);
		object[0].r.x = 25*startTime;

		while(wip_timeWindow() - startTime < 1.0/WIP_TICKRATE && !wip_globalWindow.close)
			wip_sleep(0.00001);
	}

	pthread_exit(NULL);
}

