// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Menu code

#pragma once

#include "wip_mdl.h"

typedef struct {
	int id;
	char *name;
	char *desc;
} menu_item;

typedef struct {
	char *header;
	void (*func)(unsigned int selected, void *);
	int item_c;
	menu_item *item;
} menu;

void drawMenuItem(menu_item item, unsigned int pos, unsigned int state);
void drawMenu(menu menu, unsigned int selected, wip_glmdl_t *model);

