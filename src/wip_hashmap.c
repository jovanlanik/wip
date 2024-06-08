// WIP
// Copyright (c) 2024 Jovan Lanik

// Hashmap

#include "string.h"

#include "wip_hashmap.h"

static int hash_djb2(char *s) {
	int hash = 5381;
	int c;
	while((c = *s++))
		hash = ((hash << 5) + hash) ^ c;
	return hash;
}

void *wip_hashmapGet(wip_hashmap_t *map, char *key) {
	int hash = hash_djb2(key);
	for(int i = hash % map->item_c; i < map->item_c; ++i) {
		if(strcmp(map->item[i].key, key) == 0)
			return map->item[i].value;
	}
	for(int i = 0; i < hash; ++i) {
		if(strcmp(map->item[i].key, key) == 0)
			return map->item[i].value;
	}
	return NULL;
}

int wip_hashmapInsert(wip_hashmap_t *map, char *key, void *value) {
	int hash = hash_djb2(key);
	for(int i = hash % map->item_c; i < map->item_c; ++i) {
		if(map->item[i].value != NULL) continue;
		map->item[i].key = key;
		map->item[i].value = value;
		return i;
	}
	for(int i = 0; i < hash; ++i) {
		if(map->item[i].value != NULL) continue;
		map->item[i].key = key;
		map->item[i].value = value;
		return i;
	}
	return -1;
}

