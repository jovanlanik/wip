// WIP
// Copyright (c) 2024 Jovan Lanik

// Hashmap

#pragma once

typedef struct {
	int item_c;
	struct {
		char *key;
		void *value;
	} item[];
} wip_hashmap_t;

void *wip_hashmapGet(wip_hashmap_t *map, char *key);
int wip_hashmapInsert(wip_hashmap_t *map, char *key, void *value);

