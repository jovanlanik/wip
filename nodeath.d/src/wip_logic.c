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
pthread_mutex_t wip_globalScene_m;

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

extern char wip_globalKeyName[][64];

void *wip_logicThread(void *arg) {
	wip_obj_t light, eye, center, player;
	wip_obj_t enemy[ENEMY_POOL_SIZE];

	wip_makeObject(&eye);
	eye.z = 150;
	eye.y = -10;

	wip_makeObject(&center);

	wip_makeObject(&light);
	light.x = -2;
	light.y = 4;
	light.z = 1;

	wip_makeObject(&player);
	//player.s.x = 1.5f;
	//player.s.y = 5.0f;
	{
		quat rot, cop;
		quat_dup(cop, player.rotation);
		quat_rotate(rot, TO_RAD(-40), (float[]){1, 0, 0});
		quat_mul(player.rotation, cop, rot);
		quat_norm(player.rotation, player.rotation);
	}

	//wip_makeObject(&object[0]);
	
	int sceneLength = 4 + ENEMY_POOL_SIZE;

	wip_globalScene.object = wip_alloc(sceneLength*sizeof(void *));
	wip_globalScene.object[0] = &light;
	wip_globalScene.object[1] = &eye;
	wip_globalScene.object[2] = &center;
	wip_globalScene.object[3] = &player;

	float maxH;
	float maxW;
	{
		float fov = wip_getConfFloat("game.fov") / 2.0;
		float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
		maxH = round(eye.z * sin(TO_RAD(fov)) / sinf(TO_RAD(90.0 - fov)));
		maxW = round(eye.z * sin(TO_RAD(fov * ratio))/sin(TO_RAD(90.0 - fov)));
	}
	//wip_log(WIP_INFO, "maxH: %f, maxW: %f", maxH, maxW);

	quat tilt;
	quat_identity(tilt);

	for(int i = 0; i <= ENEMY_POOL_SIZE; ++i) {
		wip_globalScene.object[i+4] = &enemy[i];
		wip_makeObject(&enemy[i]);
		enemy[i].y = (float)rand() / (float)(RAND_MAX/(2*maxH)) - 3*maxH;
		enemy[i].x = (float)rand() / (float)(RAND_MAX/(2*maxW)) - maxW;
		quat rot, cop;
		quat_dup(cop, enemy[i].rotation);
		quat_rotate(rot, TO_RAD(180), (float[]){0, 0, 1});
		quat_mul(enemy[i].rotation, cop, rot);
		quat_dup(cop, enemy[i].rotation);
		quat_rotate(rot, TO_RAD(20), (float[]){1, 0, 0});
		quat_mul(enemy[i].rotation, cop, rot);
		quat_norm(enemy[i].rotation, enemy[i].rotation);
	}

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

		for(int i = 0; i <= ENEMY_POOL_SIZE; ++i) {
			if(enemy[i].y > maxH + 2.5) {
				enemy[i].y = -maxH - 2.5;
				enemy[i].x = (float)rand() / (float)(RAND_MAX/(2*maxW)) - maxW;
			}
			else enemy[i].y += 0.15;
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

		if(wip_readMotion(SHOOT)) {
			for(int i = 0; i <= ENEMY_POOL_SIZE; ++i) {
				enemy[i].y = (float)rand() / (float)(RAND_MAX/(2*maxH)) - maxH;
				enemy[i].x = (float)rand() / (float)(RAND_MAX/(2*maxW)) - maxW;
			}
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
		vec3_scale(dir.vec, dir.vec, 0.015);
		vec3_add(player.momentum, player.momentum, dir.vec);

		// TODO: write a function for this
		quat rot, cop;
		quat_dup(cop, player.rotation);
		quat_dup(rot, tilt);
		quat_mul(player.rotation, cop, rot);
		quat_dup(cop,player.rotation);
		quat_rotate(rot, TO_RAD(50 * player.m.x), (float[]){0, 1, 0});
		quat_mul(player.rotation, cop, rot);
		quat_norm(player.rotation, player.rotation);
		quat_conj(tilt, rot);
		//wip_debug(WIP_INFO, "%f", player.r.x);

		vec3_add(player.position, player.position, player.momentum);
		vec3_scale(player.momentum, player.momentum, 0.95);

		if(fabs(player.y) >= maxH-1) player.m.y *= 1 - pow((maxH+1 - fabs(player.y))/2.0 - 1, 2);
		if(player.y > maxH) player.y = maxH;
		if(player.y < -maxH) player.y = -maxH;
		if(fabs(player.x) >= maxW-1) player.m.x *= 1 - pow((maxW+1 - fabs(player.x))/2.0 - 1, 2);
		if(player.x > maxW) player.x = maxW;
		if(player.x < -maxW) player.x= -maxW;

		while(wip_timeWindow() - startTime < 1.0/WIP_TICKRATE && !wip_globalWindow.close)
			wip_sleep(0.00001);
	}

	pthread_exit(NULL);
}

