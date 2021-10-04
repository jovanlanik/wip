//
// Dungeon
// Copyright (c) 2021 Jovan Lanik
//

// Dungeon format

enum direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum tile {
	WALL,
	FLOOR,
	DOOR,
	GATE
};

enum deco {
	NONE,
	WALL,
	FLOOR,

};

typedef struct {
	int width, height;
	tile_t **tile;
	deco_t **deco;
} room_t;

typedef struct {
	int room_c;
	room_t *room;
} dungeon_t;

openDungeon(dungeon_t *dungeon, const char *filename);
freeDungeon(dungeon_t *dungeon);
