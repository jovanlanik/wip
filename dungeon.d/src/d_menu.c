// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Menu code

#include <string.h>
#include <GL/glew.h>

#include "d_menu.h"
#include "d_text.h"

#define MARGIN 10
#define HEADER_SCALE 16
#define ITEM_SCALE 8
#define DESC_SCALE 4

void drawMenuItem(menu_item item, unsigned int pos, unsigned int state) {
	drawFormatStr(MARGIN, 2*MARGIN + pos*(ITEM_SCALE*CHAR_SIZE + MARGIN + DESC_SCALE*CHAR_SIZE) +
		CHAR_SIZE*HEADER_SCALE, ITEM_SCALE, "%c %s", state == 0 ? '-' : '+', item.name);
	if(item.desc && item.desc[0] != '\0') {
		drawFormatStr(MARGIN, 2*MARGIN + pos*(DESC_SCALE*CHAR_SIZE + MARGIN + ITEM_SCALE*CHAR_SIZE) +
			HEADER_SCALE*CHAR_SIZE + ITEM_SCALE*CHAR_SIZE, DESC_SCALE, "%s", item.desc);
	}
	return;
}

void drawMenu(menu menu, unsigned int selected) {
	drawStr(MARGIN, MARGIN, HEADER_SCALE, menu.header);
	for(int i = 0; i < menu.item_c; ++i) {
		drawMenuItem(menu.item[i], i, i == selected);
	}
}

