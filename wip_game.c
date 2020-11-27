//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Game Code

#include <stddef.h>
#include <math.h>
#include <pthread.h>
#include <GLFW/glfw3.h>

#include "wip_fn.h"
#include "wip_obj.h"
#include "wip_window.h"
#include "wip_input.h"
#define WIP_GAME
#include "wip_game.h"
wip_scene_t wip_globalScene;
pthread_mutex_t wip_globalScene_m;

void *wip_logicThread(void *arg) {
	wip_obj_t object[1];
	wip_obj_t light, eye, center;

	wip_makeObject(&center);

	wip_makeObject(&eye);
	eye.y = -3;

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
		if(key.key == GLFW_KEY_Q) {
			pthread_mutex_lock(&wip_globalWindow_m);
			wip_globalWindow.close = 1;
			pthread_mutex_unlock(&wip_globalWindow_m);
		}

		object[0].z = 0.5*sin(wip_timeWindow(&wip_globalWindow));
		object[0].r.x = 25*wip_timeWindow(&wip_globalWindow);
	}

	pthread_exit(NULL);
}
