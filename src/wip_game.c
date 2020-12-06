//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Game Code

#include <stddef.h>
#include <math.h>
#include <pthread.h>

#include "wip_fn.h"
#include "wip_obj.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_game.h"

wip_scene_t wip_globalScene;
pthread_mutex_t wip_globalScene_m;

extern wip_window_t wip_globalWindow;
extern pthread_mutex_t wip_globalWindow_m;

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

	while(!wip_globalWindow.close) {
		wip_key_t key = wip_readKey();
		//if(key.key) wip_debug(WIP_INFO, "Key: %d - %c", key.key, key.key);
		if(key.key == WIP_ESC || key.key == 'q') {
			pthread_mutex_lock(&wip_globalWindow_m);
			wip_globalWindow.close = 1;
			pthread_mutex_unlock(&wip_globalWindow_m);
		}
		if(key.action) {
			if(key.key == WIP_U) {
				eye.y -= eye.y - center.y;
				eye.x -= eye.x - center.x;
			}
			if(key.key == WIP_D) {
				eye.y += eye.y - center.y;
				eye.x += eye.x - center.x;
			}
			if(key.key == WIP_L) eye.r.y -= 0.1;
			if(key.key == WIP_R) eye.r.y += 0.1;
			getPointOnCircle(&center, &eye, eye.r.y, 1);
		}

		object[0].z = 0.5*sin(wip_timeWindow());
		object[0].r.x = 25*wip_timeWindow();
	}

	pthread_exit(NULL);
}
