// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon format

#pragma once

#include "wip_mdl.h"

#include "dungeon.h"
#include "d_state.h"

enum tile {
	TILE_FLOOR,
	TILE_WALL,
	TILE_DOOR,
	TILE_GATE,
};

typedef struct {
	enum tile type;
	unsigned int id;
	void *data;
} tile_t;

typedef struct {
	enum direction dir;
	wip_glmdl_t *model;
} deco_t;

typedef struct {
	unsigned int width, height;
	unsigned int deco_c;
	tile_t *tile;
	deco_t **deco;
} room_t;

typedef struct {
	unsigned int room_c;
	room_t *room;
} dungeon_t;

int readDungeon(dungeon_t *dungeon, state_t *state, char *filename);
//freeDungeon(dungeon_t *dungeon);
