// Dungeon
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

#include "d_player.h"

int wip_globalTicksPerSecond;

wip_scene_t wip_globalScene;

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

extern char wip_globalKeyName[][64];

extern float m[];

void *wip_logicThread(void *arg) {
	wip_obj_t center, camera;
	player_t play;
	play.x = 0;
	play.y = 0;
	play.d = DIR_NORTH;

	wip_makeObject(&camera);
	camera.y = 0.0f;
	camera.z = 0.0f;

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
			//if(key.key == WIP_SPACE) wip_log(WIP_INFO, "%f %f %f", m[0], m[1], m[2]);
			if(key.key == WIP_SPACE && key.action == WIP_PRESS) {
				wip_log(WIP_INFO, "%f, %f", camera.x, camera.y);
				wip_log(WIP_INFO, "%f, %f", center.x, center.y);
				wip_print("");
			}
			wip_writeMotion(key);
		}

		if(wip_readMotion(UP)) {
			play.x += abs(play.d%4-2)-1;
			play.y += abs(play.d+1%4-2)-1;
		}
		if(wip_readMotion(DOWN)) {
			play.x -= abs(play.d%4-2)-1;
			play.y -= abs(play.d+1%4-2)-1;
		}
		if(wip_readMotion(RIGHT)) play.d++;
		if(wip_readMotion(LEFT)) play.d--;
		play.d = play.d % 4;
		/*
		if(wip_readMotion(UP)) camera.x += .1f;
		if(wip_readMotion(DOWN)) camera.x -= .1f;
		if(wip_readMotion(LEFT)) camera.y += .1f;
		if(wip_readMotion(RIGHT)) camera.y -= .1f;
		*/
		/*
		const float step = 0.025;
		if(wip_readMotion(UP)) m[0] += step;
		if(wip_readMotion(DOWN)) m[0] -= step;
		if(wip_readMotion(LEFT)) m[1] -= step;
		if(wip_readMotion(RIGHT)) m[1] += step;
		if(wip_readMotion(OK)) m[2] += step;
		if(wip_readMotion(CANCEL)) m[2] -= step;
		*/

		// TODO: disable this when you fix the material rendering
		//quat rot, cop;
		//quat_dup(cop, center.rotation);
		//quat_rotate(rot, TO_RAD(0.25f), (float[]){0, 0, 1});
		//quat_mul(center.rotation, cop, rot);
		//quat_norm(center.rotation, center.rotation);

		center.x = 2*play.x;
		center.y = 2*play.y;
		camera.x = center.x - abs(play.d%4-2)+1;
		camera.y = center.y - abs(play.d+1%4-2)+1;

		while(wip_timeWindow() - startTime < 1.0/WIP_TICKRATE && !wip_globalWindow.close)
			wip_sleep(0.00001);
	}

	pthread_exit(NULL);
}

