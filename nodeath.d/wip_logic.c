//
// WIP
// Copyright (c) 2021 Jovan Lanik
//

// Game Logic

#include <string.h>
#include <stddef.h>
#include <math.h>
#include <pthread.h>

#include "wip_fn.h"
#include "wip_obj.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_game.h"

#include "lib/linmath.h"

int wip_globalTicksPerSecond;

wip_scene_t wip_globalScene;
pthread_mutex_t wip_globalScene_m;

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

extern char wip_globalKeyName[][64];

void *wip_logicThread(void *arg) {
	//wip_obj_t object[1];
	wip_obj_t light, eye, center, player;

	wip_makeObject(&eye);
	eye.z = 70;
	eye.y = -10;

	wip_makeObject(&center);

	wip_makeObject(&light);
	light.x = -2;
	light.y = 4;
	light.z = 1;

	wip_makeObject(&player);
	player.r.x = -30;

	//wip_makeObject(&object[0]);

	wip_globalScene.object = wip_alloc(4*sizeof(void *));
	wip_globalScene.object[0] = &light;
	wip_globalScene.object[1] = &eye;
	wip_globalScene.object[2] = &center;
	wip_globalScene.object[3] = &player;
	//wip_globalScene.object[4] = &object[0];
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

		union WIP_NAMED_VEC_T(3, float, WIP_XYZ, vec, ) dir = { .x = 0.0, .y = 0.0, .z = 0.0 };

		if(wip_readMotion(MOVE_UP)) {
			dir.y += 1;
		}
		if(wip_readMotion(MOVE_DOWN)) {
			dir.y -= 1;
		}
		if(wip_readMotion(MOVE_LEFT)) {
			dir.x -= 1;
		}
		if(wip_readMotion(MOVE_RIGHT)) {
			dir.x += 1;
		}

		if(vec3_len(dir.vec) != 0) vec3_norm(dir.vec, dir.vec);
		vec3_scale(dir.vec, dir.vec, 0.01);
		vec3_add(player.momentum, player.momentum, dir.vec);

		player.r.y = 50 * player.m.x;

		vec3_add(player.position, player.position, player.momentum);
		vec3_scale(player.momentum, player.momentum, 0.95);

		while(wip_timeWindow() - startTime < 1.0/WIP_TICKRATE && !wip_globalWindow.close)
			wip_sleep(0.00001);
	}

	pthread_exit(NULL);
}
