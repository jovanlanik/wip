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
#include "wip_game.h"

void *wip_logicThread(void *arg) {
	wip_obj_t object[2];
	wip_obj_t light, eye, center;

	wip_makeObject(&center);

	wip_makeObject(&eye);
	eye.y = -3;

	wip_makeObject(&light);
	light.x = -2;
	light.y = -3;
	light.z = 2;

	wip_makeObject(&object[0]);
	object[0].x = -1.5;
	wip_makeObject(&object[1]);
	object[1].x = 1.5;

	wip_globalScene.object = wip_alloc(5*sizeof(void *));
	wip_globalScene.object[0] = &light;
	wip_globalScene.object[1] = &eye;
	wip_globalScene.object[2] = &center;
	wip_globalScene.object[3] = &object[0];
	wip_globalScene.object[4] = &object[1];
	wip_globalScene.length = 5;

	while(!wip_globalWindow.close) {
		wip_key_t key = wip_readKey();
		if(key.key == GLFW_KEY_Q) {
			pthread_mutex_lock(&wip_globalWindow_m);
			wip_globalWindow.close = 1;
			pthread_mutex_unlock(&wip_globalWindow_m);
		}

		object[0].z = 0.5*sin(glfwGetTime());
		object[0].r.x = 25*glfwGetTime();
	}

	pthread_exit(NULL);
}
