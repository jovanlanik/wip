// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon format

#include "wip_fn.h"

#include "d_format.h"

room_t *testRoom(void) {
	static wip_glmdl_t *wall_model = NULL;
	static wip_glmdl_t *floor_model = NULL;
	if(!wall_model) wall_model = wip_openModel("d_wall");
	if(!floor_model) floor_model = wip_openModel("d_floor");
	room_t *room = wip_alloc(sizeof(room_t));
	room->width = 10;
	room->height = 10;
	room->tile = NULL;
	room->deco = wip_alloc(sizeof(deco_t**[10]));
	for(int i = 0; i < 10; ++i) {
		room->deco[i] = wip_alloc(sizeof(deco_t*[10]));
		for(int n = 0; n < 10; ++n) {
			if(i % 9 == 0 || n % 9 == 0) {
				room->deco[i][n] = wip_alloc(sizeof(struct deco_model));
				((struct deco_model*)room->deco[i][n])->type = DECO_MODEL;
				((struct deco_model*)room->deco[i][n])->model = wall_model;
			}
			else {
				room->deco[i][n] = wip_alloc(sizeof(struct deco_model));
				((struct deco_model*)room->deco[i][n])->type = DECO_MODEL;
				((struct deco_model*)room->deco[i][n])->model = floor_model;
			}
		}
	}
	return room;
}
