// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Dungeon format

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wip_fn.h"

#include "d_format.h"

room_t *testRoom(void) {
	wip_glmdl_t *wall_model = wip_openModel("d_wall");
	wip_glmdl_t *floor_model = wip_openModel("d_floor");
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
				((struct deco_model *)room->deco[i][n])->type = DECO_MODEL;
				((struct deco_model *)room->deco[i][n])->model = wall_model;
			}
			else {
				room->deco[i][n] = wip_alloc(sizeof(struct deco_model));
				((struct deco_model *)room->deco[i][n])->type = DECO_MODEL;
				((struct deco_model *)room->deco[i][n])->model = floor_model;
			}
		}
	}
	return room;
}

/*
int readDungeon(dungeon_t *dungeon, const char *filename) {
	wip_debug(WIP_INFO, "%s: Loading dungeon from %s...", __func__, filename);
	dungeon->room_c = 0;
	dungeon->room = NULL;

	int token_c = 0;
	char **token = wip_alloc(sizeof(void *));

	char *line = NULL;
	size_t len;
	ssize_t read;
	FILE *file = wip_openFile(filename);
	while((read = getline(&line, &len, file)) != -1) {
		if(line[0] == '#') continue;
		const char search[] = " \t\n";
*/
		/*
			Who knew that C had a function for splitting strings into tokens?
			Only took misspeling `man strtol` as `man strtok` to find out...
		*/
/*
		for(char *word = strtok(line, search); word != NULL; word = strtok(NULL, search)) {
			token = wip_realloc(token, (token_c+1)*sizeof(void *), NULL);
			token[token_c] = strdup(word);
			token_c++;
		}
	}
	wip_free(line);
	wip_debug(WIP_INFO, "%s: Found %d tokens in dungeon file.", __func__, token_c);

	for(int i = 0; i < token_c; ++i) {
		if(strcmp("room", token[i]) == 0) {
			unsigned int id;
			if(wip_atoui(token[i+1], &id)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected uint.", __func__, token[i+1]);
				return 1;
			}

			if(id >= dungeon->room_c) {
				dungeon->room_c = id;
				dungeon->room = wip_realloc(dungeon->room, (id+1)*sizeof(room_t), NULL);
			}

			if(wip_atoui(token[i+2], &dungeon->room[id].width)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected uint.", __func__, token[i+2]);
				return 1;
			}
			if(wip_atoui(token[i+3], &dungeon->room[id].height)) {
				wip_log(WIP_ERROR, "%s: Unexpected token: %s, expected uint.", __func__, token[i+3]);
				return 1;
			}

			wip_debug(WIP_INFO, "%s: Found room with id %d.", __func__, id);
			i += 3;
		}
		else if(strcmp("deco", token[i]) == 0) {
			wip_debug(WIP_INFO, "%s: Found deco for room %s, layer %s.", __func__, token[i+1], token[i+2]);
			i += 2;
		}
		//else wip_debug(WIP_ERROR, "%s: Unknown token: %s.", __func__, token[i]);
	}

	if(dungeon->room_c == 0)
		wip_debug(WIP_WARN, "%s: Couldn't load any rooms from dungeon, may be missing.", __func__);
	wip_debug(WIP_INFO, "%s: Done.", __func__);
	return 0;
}
*/

