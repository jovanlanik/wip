// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Player

#pragma once

#include <stdio.h>

#include "wip_types.h"
#include "dungeon.h"

#define ENT_MAX 128

typedef struct {
	union WIP_NAMED_VEC_T(2, int, WIP_XY, position, );
	union { enum direction d; enum direction direction; };
	int health;
} player_t;

enum entity {
	ENT_NONE,
	ENT_BOOK,
	ENT_KEY,
	ENT_HEAL,
	ENT_COBRA,
	ENT_SNAKE,
};

typedef struct {
	unsigned int room;
	enum entity type;
	int id;
	union WIP_NAMED_VEC_T(2, int, WIP_XY, position, );
	union { enum direction d; enum direction direction; };
} entity_t;

typedef struct {
	char dungeon[FILENAME_MAX];
	unsigned int room;
	unsigned int keyring[9];
	entity_t entity[ENT_MAX];
	player_t player;
} state_t;
