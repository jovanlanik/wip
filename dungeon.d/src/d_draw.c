// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Drawing functions

#include "wip_mdl.h"
#include "wip_obj.h"
#include "wip_img.h"
#include "wip_math.h"
#include "external/glad/glad.h"
#include "external/linmath.h"

#include "d_format.h"

extern unsigned int lightLocation;
extern unsigned int mpvLocation;
extern unsigned int transformLocation;
extern unsigned int normalTransformLocation;
extern unsigned int materialLocation;

extern GLuint program;

extern float m[3];

void drawModel(wip_obj_t *object, wip_glmdl_t *model, wip_globj_t pv, wip_obj_t *light) {
	static int init = 0;
	static GLuint texture;
	if(init == 0) {
		init = 1;
		texture = wip_openTexture("d_wall");
	}

	if(!model) return;

	wip_globj_t mpv;
	wip_globj_t transform;
	wip_loadObject(&transform, object);
	wip_globj_t normalTransform;
	{
		wip_globj_t go;
		mat4x4_invert(go.m, transform.m);
		mat4x4_transpose(normalTransform.m, go.m);
	}
	mat4x4_mul(mpv.m, pv.m, transform.m);
	wip_loadObjectF(WIP_POS, &transform, object);

	glBindVertexArray(model->vertex_a);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glUseProgram(program);
	if(light) glUniform3fv(lightLocation, 1, light->position);
	glUniformMatrix4fv(mpvLocation, 1, GL_FALSE, mpv.f);
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, transform.f);
	glUniformMatrix4fv(normalTransformLocation, 1, GL_FALSE, normalTransform.f);
	glUniform3fv(materialLocation, 1, m);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, model->element_c, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	return;
}

void drawRoom(room_t *room, wip_globj_t pv) {
	static int init = 0;
	static wip_obj_t object[4];
	if(init == 0) {
		init = 1;
		for(int i = 0; i < 4; ++i) {
			wip_makeObject(&object[i]);
			quat_rotate(object[i].rotation, TO_RAD(i * 90.0), (float[]){0, 0, 1});
		}
	}
	unsigned int w = room->width;
	for(int layer = 0; layer < room->deco_c; ++layer) {
		for(int y = 0; y < room->height; ++y) {
			for(int x = 0; x < room->width; ++x) {
				if(room->deco[layer][w*y+x].model) {
					object[room->deco[layer][w*y+x].dir].x = 2*x;
					object[room->deco[layer][w*y+x].dir].y = 2*y;
					drawModel(
						&object[room->deco[layer][w*y+x].dir],
						room->deco[layer][w*y+x].model, pv, NULL
					);
				}
			}
		}
	}
	return;
}

static wip_glmdl_t *getEntModel(entity_t *ent) {
	static int init = 0;
	static wip_glmdl_t *book_model;
	static wip_glmdl_t *snake_model;
	static wip_glmdl_t *cobra_model;
	static wip_glmdl_t *key_model;
	static wip_glmdl_t *heal_model;
	if(init == 0) {
		init = 1;
		snake_model = wip_openModel("d_snake");
		key_model = wip_openModel("d_key");
		book_model = wip_openModel("d_book");
		cobra_model = wip_openModel("d_cobra");
		heal_model = wip_openModel("d_heal");
	}
	switch(ent->type) {
		case ENT_BOOK:
			return book_model;
		case ENT_KEY:
			return key_model;
		case ENT_COBRA:
			return cobra_model;
		case ENT_SNAKE:
			return snake_model;
		case ENT_HEAL:
			return heal_model;
		default:
			return NULL;
	}
}

void drawEnts(unsigned int room, entity_t *ent, wip_globj_t pv) {
	static int init = 0;
	static wip_obj_t object[4];
	if(init == 0) {
		init = 1;
		for(int i = 0; i < 4; ++i) {
			wip_makeObject(&object[i]);
			quat_rotate(object[i].rotation, TO_RAD(i * 90.0), (float[]){0, 0, 1});
		}
	}
	for(int i = 0; i < ENT_MAX; ++i) {
		if(ent[i].type == ENT_NONE || ent[i].room != room) continue;
		object[ent[i].direction].x = 2 * ent[i].x;
		object[ent[i].direction].y = 2 * ent[i].y;
		wip_glmdl_t *model = getEntModel(&ent[i]);
		drawModel(
			&object[ent[i].direction],
			model, pv, NULL
		);
	}
}

