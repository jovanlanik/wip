// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Menu code

#include <string.h>

#include "wip_obj.h"
#include "wip_conf.h"
#include "wip_math.h"
#include "external/glad/glad.h"
#include "external/linmath.h"

#include "d_menu.h"
#include "d_text.h"
#include "d_draw.h"

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

void drawMenu(menu menu, unsigned int selected, wip_glmdl_t *model) {
	static int init = 0;
	const vec3 axis = {0.0f, 0.0f, 1.0f};
	static wip_obj_t center, camera, offset;
	static wip_globj_t pv, view;
	static wip_globj_t projection;
	static wip_glmdl_t *default_model;

	if(init == 0) {
		init = 1;
		wip_makeObject(&center);
		wip_makeObject(&camera);
		wip_makeObject(&offset);
		camera.y -= 2.75;
		offset.x += 1.25;
		float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
		mat4x4_perspective(projection.m, TO_RAD(90.0), ratio, 0.1, 1000);
		mat4x4_look_at(view.m, camera.position, center.position, (float *)axis);
		mat4x4_mul(pv.m, projection.m, view.m);
		default_model = wip_openModel("d_cobra");
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	drawModel(&offset, model ? model : default_model, pv, NULL);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawStr(MARGIN, MARGIN, HEADER_SCALE, menu.header);
	for(int i = 0; i < menu.item_c; ++i) {
		drawMenuItem(menu.item[i], i, i == selected);
	}
}

