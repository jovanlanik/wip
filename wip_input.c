//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Input Functions

#include <unistd.h>

#include "wip_input.h"

int wip_keyHead, wip_keyTail;
wip_key_t wip_keyBuffer[WIP_KEY_BUFFER];

// TODO: clean up

int wip_keyWrite(wip_key_t key) {
	int a;
	for(a = WIP_INPUT_ATTEMPTS; a > 0; --a)
		if(wip_keyHead + 1 != wip_keyTail) break;
		else sleep(WIP_INPUT_SLEEP);
	if(a == 0) return 0;
	wip_keyHead = (wip_keyHead + 1) % WIP_KEY_BUFFER;
	wip_keyBuffer[wip_keyHead] = key;
	return 1;
}

wip_key_t wip_keyRead(void) {
	if(wip_keyHead == wip_keyTail) return (wip_key_t){ 0, -1 };
	wip_keyTail = (wip_keyTail + 1) % WIP_KEY_BUFFER;
	return wip_keyBuffer[wip_keyTail];
}

