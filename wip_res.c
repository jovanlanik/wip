//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Resource Management Functions

#include <stddef.h>
#include <pthread.h>

void *wip_resourceThread(void *arg) {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_exit(NULL);
}
