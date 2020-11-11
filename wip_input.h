//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Input Functions

#define WIP_INPUT_ATTEMPTS 4
#define WIP_INPUT_SLEEP 0.01
#define WIP_KEY_BUFFER 16

typedef struct {
	unsigned int action : 1;
	int key : 31;
} wip_key_t;

int wip_keyWrite(wip_key_t k);
wip_key_t wip_keyRead(void);

