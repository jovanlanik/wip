// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon format

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "wip_fn.h"

#include "d_format.h"

#ifdef _WIN32
static int getline(char **lineptr, size_t *n, FILE *stream) {
	if(!lineptr || !n || !stream) {
		errno = EINVAL;
		return -1;
	}
	if(!*lineptr) *lineptr = malloc(*n = 128);
	int read = 0;
	while(1) {
		while(read < *n-1) {
			char c = fgetc(stream);
			if(c == EOF) return -1;
			(*lineptr)[read] = c;
			if(c == '\n') {
				(*lineptr)[read+1] = '\0';
				return read;
			}
			read++;
		}
		*n = *n * 2;
		wip_debug(WIP_INFO, "%s: Reallocating buffer to %d bytes.", __func__, *n);
		int ret = 0;
		*lineptr = wip_realloc(*lineptr, *n, &ret);
		if(!ret) return -1;
	}
}
#endif

static const enum direction direction_map[] = {
	['0'] = DIR_NORTH,
	['1'] = DIR_EAST,
	['2'] = DIR_SOUTH,
	['4'] = DIR_WEST,
	['N'] = DIR_NORTH,
	['E'] = DIR_EAST,
	['S'] = DIR_SOUTH,
	['W'] = DIR_WEST,
	['^'] = DIR_NORTH,
	['>'] = DIR_EAST,
	['v'] = DIR_SOUTH,
	['<'] = DIR_WEST,
};

static int model_init = 0;
static wip_glmdl_t *floor_model;
static wip_glmdl_t *wall_model;
static wip_glmdl_t *door_model;
static wip_glmdl_t *gate_model;
static wip_glmdl_t *model['Z'-'A'];

static void modelInit(void) {
		model_init = 1;
		floor_model = wip_openModel("d_floor");
		wall_model = wip_openModel("d_wall");
		door_model = wip_openModel("d_door");
		gate_model = wip_openModel("d_gate");
}


// TODO: Door and Gate model rotation
// TODO: Custom tile types (using lua?)
static int readRoom(char **token, room_t *room) {
	if(model_init == 0) modelInit();

	int i;
	for(i = 0; i < room->width * room->height; ++i) {
		switch(token[i][0]) {
			case '-':
				room->tile[i].type = TILE_FLOOR;
				room->deco[0][i].model = floor_model;
				room->deco[0][i].dir = DIR_NORTH;
				break;
			case '#':
				room->tile[i].type = TILE_WALL;
				room->deco[0][i].model = wall_model;
				room->deco[0][i].dir = DIR_NORTH;
				break;
			case 'D':
				room->tile[i].type = TILE_DOOR;
				room->deco[0][i].model = door_model;
				room->deco[0][i].dir = DIR_NORTH;
				if(wip_atoui(&token[i][1], &room->tile[i].id)) {
					wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected key id (uint).", __func__, &token[i][1]);
					return -1;
				}
				break;
			case 'G':
				room->tile[i].type = TILE_GATE;
				room->deco[0][i].model = gate_model;
				room->deco[0][i].dir = DIR_NORTH;
				if(wip_atoui(&token[i][1], &room->tile[i].id)) {
					wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected room id (uint).", __func__, &token[i][1]);
					return -1;
				}
				break;
			default:
				wip_log(WIP_ERROR, "%s: Unknown tile: %s.", __func__, token[i]);
				return -1;
		}
	}
	return i;
}

// TODO: Custom models
static int readDeco(char **token, room_t *room, unsigned int layer) {
	if(model_init == 0) modelInit();

	int i;
	for(i = 0; i < room->width * room->height; ++i) {
		switch(token[i][0]) {
			case '=':
				if(layer == 0) break;
			case '.':
				room->deco[layer][i].model = NULL;
				break;
			case '-':
				room->deco[layer][i].model = floor_model;
				room->deco[0][i].dir = DIR_NORTH;
				break;
			case '#':
				room->deco[layer][i].model = wall_model;
				room->deco[0][i].dir = DIR_NORTH;
				break;
			case 'D':
				room->deco[layer][i].model = door_model;
				room->deco[0][i].dir = DIR_NORTH;
				break;
			default:
				if(model[(int)token[i][0]-'A']) room->deco[layer][i].model = model[(int)token[i][0]-'A'];
				else {
					wip_log(WIP_ERROR, "%s: Unknown deco: %s.", __func__, token[i]);
					return -1;
				}
		}
		if(token[i][0] != token[i][1]) room->deco[layer][i].dir = direction_map[(int)token[i][1]];
	}
	return i;
}

int readDungeon(dungeon_t *dungeon, state_t *state, char *filename) {
	wip_debug(WIP_INFO, "%s: Loading dungeon from %s...", __func__, filename);

	size_t size = 128;
	int token_c = 0;
	char **token = wip_alloc(sizeof(void *) * size);

	char *line = NULL;
	size_t len;
	int read;
	FILE *file = wip_openFile(filename);
	while((read = getline(&line, &len, file)) != -1) {
		if(line[0] == '!') continue;
		const char search[] = " \t\n";
		/*
			Who knew that C had a function for splitting strings into tokens?
			Only took misspeling `man strtol` as `man strtok` to find out...
		*/
		for(char *word = strtok(line, search); word != NULL; word = strtok(NULL, search)) {
			if(token_c > size) {
				size *= 2;
				token = wip_realloc(token, sizeof(void *) * size, NULL);
				wip_debug(WIP_INFO, "%s: Reallocating buffer to %d bytes.", __func__, size);
			}
			token[token_c] = strdup(word);
			token_c++;
		}
	}
	wip_free(line);
	wip_debug(WIP_INFO, "%s: Found %d tokens in dungeon file.", __func__, token_c);

	state->dungeon = filename;
	state->room = 0;
	state->player.x = 0;
	state->player.y = 0;
	state->player.direction = DIR_NORTH;

	dungeon->room_c = 0;
	dungeon->room = NULL;

	for(int i = 0; i < token_c; ++i) {
		if(strcmp("room", token[i]) == 0) {
			unsigned int id;
			if(wip_atoui(token[i+1], &id)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected room id (uint).", __func__, token[i+1]);
				return 1;
			}
			if(id >= dungeon->room_c) {
				dungeon->room_c = id+1;
				dungeon->room = wip_realloc(dungeon->room, sizeof(room_t) * dungeon->room_c, NULL);
				dungeon->room[id].deco_c = 1;
				dungeon->room[id].deco = wip_alloc(sizeof(deco_t*));
				dungeon->room[id].deco[0] = NULL;
			}
			wip_debug(WIP_INFO, "%s: Found room with id %d.", __func__, id);
			i += 4;
		}
		else if(strcmp("deco", token[i]) == 0) {
			unsigned int id, layer;
			if(wip_atoui(token[i+1], &id)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected room id (uint).", __func__, token[i+1]);
				return 1;
			}
			if(id >= dungeon->room_c) {
				dungeon->room_c = id+1;
				dungeon->room = wip_realloc(dungeon->room, sizeof(room_t) * dungeon->room_c, NULL);
				dungeon->room[id].deco_c = 1;
				dungeon->room[id].deco = wip_alloc(sizeof(deco_t*));
				dungeon->room[id].deco[0] = NULL;
			}
			if(wip_atoui(token[i+2], &layer)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected room id (uint).", __func__, token[i+1]);
			}
			if(layer >= dungeon->room[id].deco_c) {
				dungeon->room[id].deco_c = layer+1;
				dungeon->room[id].deco = wip_realloc(dungeon->room[id].deco, sizeof(deco_t*) * dungeon->room[id].deco_c, NULL);
				dungeon->room[id].deco[layer] = NULL;
			}
			wip_debug(WIP_INFO, "%s: Found deco for room %d, layer %d.", __func__, id, layer);
			i += 3;
		}
		else if(strcmp("model", token[i]) == 0 && token[i+1][1] == '\0' && token[i+1][0] > 'A' && token[i+1][0] < 'Z') {
			model[(int)token[i+1][0]-'A'] = wip_openModel(token[i+2]);
			i += 2;
		}
		else if(strcmp("player", token[i]) == 0) i += 4;
	}

	for(int i = 0; i < token_c; ++i) {
		if(strcmp("room", token[i]) == 0) {
			unsigned int id;
			if(wip_atoui(token[i+1], &id)) return 1;
			if(wip_atoui(token[i+2], &dungeon->room[id].width)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected width (uint).", __func__, token[0]);
				return 1;
			}
			if(wip_atoui(token[i+3], &dungeon->room[id].height)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected height (uint).", __func__, token[1]);
				return 1;
			}
			i += 4;

			dungeon->room[id].tile = wip_alloc(sizeof(tile_t) * dungeon->room[id].width * dungeon->room[id].height);
			if(dungeon->room[id].deco[0] == NULL)
				dungeon->room[id].deco[0] = wip_alloc(sizeof(deco_t) * dungeon->room[id].width * dungeon->room[id].height);

			int ret = readRoom(&token[i], &dungeon->room[id]);
			if(ret == -1) return 1;
			i += ret-1;
		}
		else if(strcmp("deco", token[i]) == 0) {
			unsigned int id, layer;
			if(wip_atoui(token[i+1], &id)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected room id (uint).", __func__, token[i+1]);
				return 1;
			}
			if(wip_atoui(token[i+2], &layer)) return 1;
			i += 3;

			if(dungeon->room[id].deco[layer] == NULL)
				dungeon->room[id].deco[layer] = wip_alloc(sizeof(deco_t) * dungeon->room[id].width * dungeon->room[id].height);

			int ret = readDeco(&token[i], &dungeon->room[id], layer);
			if(ret == -1) return 1;
			i += ret-1;
		}
		else if(strcmp("model", token[i]) == 0) i += 2;
		else if(strcmp("player", token[i]) == 0) { 
			unsigned int id;
			if(wip_atoui(token[i+1], &id)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected room id (uint).", __func__, token[i+1]);
				return 1;
			}
			else if(id < dungeon->room_c) state->room = id;
			if(wip_atoi(token[i+2], &state->player.x)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected X pos (int).", __func__, token[i+1]);
				return 1;
			}
			if(wip_atoi(token[i+3], &state->player.y)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected Y pos (int).", __func__, token[i+1]);
				return 1;
			}
			state->player.direction = direction_map[(int)token[i+4][0]];
			i += 4;
		}
		else {
			wip_debug(WIP_ERROR, "%s: Unknown token: %s.", __func__, token[i]);
			return 1;
		}
	}

	if(dungeon->room_c == 0)
		wip_debug(WIP_WARN, "%s: Couldn't load any rooms from dungeon, may be missing.", __func__);
	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return 0;
}

