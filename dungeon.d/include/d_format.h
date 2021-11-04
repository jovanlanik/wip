// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon format

#pragma once

#include "wip_mdl.h"

#include "dungeon.h"

enum tile {
	TILE_WALL,
	TILE_FLOOR,
	TILE_DOOR,
	TILE_GATE
};

typedef enum tile tile_t;

struct tile_wall { tile_t type; };
struct tile_floor { tile_t type; };

struct tile_door {
	tile_t type;
	int key_id;
};

struct tile_gate {
	tile_t type;
	int key_id, room_id;
};

enum deco {
	DECO_NONE,
	DECO_MODEL,
	DECO_ROOM,
};

typedef enum deco deco_t;

struct deco_none { deco_t type; };

struct deco_model {
	deco_t type;
	enum direction dir;
	// TODO: is glmdl realy the type to be used?
	wip_glmdl_t *model;
};

struct deco_room {
	deco_t type;
	// TODO: room deco struct definition
};

typedef struct {
	int width, height;
	tile_t ***tile;
	deco_t ***deco;
} room_t;

typedef struct {
	int room_c;
	room_t *room;
} dungeon_t;

room_t *testRoom(void);
dungeon_t *readDungeon(dungeon_t *dungeon, const char *filename);
//freeDungeon(dungeon_t *dungeon);
