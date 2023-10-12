// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Drawing functions

#include "wip_gl.h"
#include "wip_mdl.h"
#include "wip_obj.h"
#include "wip_img.h"
#include "wip_math.h"
#include "external/glad/glad.h"
#include "external/linmath.h"

#include "d_draw.h"
#include "d_format.h"

extern unsigned int lightLocation;
extern unsigned int mpvLocation;
extern unsigned int transformLocation;
extern unsigned int normalTransformLocation;
extern unsigned int materialLocation;

extern GLuint program;

extern const char _binary_d_background_vert_start[];
extern const char _binary_d_background_frag_start[];

extern float m[3];

#ifdef GL_TRUE
#define UINT GLuint
#else
#define UINT uint32_t
#endif
#define BUFF2(type) { type data_b; type element_b; }

void drawScreen(float rot) {
	const float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	const unsigned int indices[] = {
		0, 1, 2, 3,
	};

	static int init = 0;
	static union WIP_NAMED_VEC_T(2, UINT, BUFF2, buffers, ) render_object;
	static GLuint vertex_a = 0;
	static GLuint program = 0;
	static GLuint dirLocation = 0;
	static GLuint timeLocation = 0;
	if(init == 0) {
		init = 1;

		glGenVertexArrays(1, &vertex_a);
		glBindVertexArray(vertex_a);

		glGenBuffers(2, render_object.buffers);
		glBindBuffer(GL_ARRAY_BUFFER, render_object.data_b);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_object.element_b);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		GLuint vert = wip_loadShader((char *)_binary_d_background_vert_start, GL_VERTEX_SHADER);
		GLuint frag = wip_loadShader((char *)_binary_d_background_frag_start, GL_FRAGMENT_SHADER);
		program = wip_loadProgram(vert, frag);
		for(int i = 0; i < 25; ++i) wip_printGlErrors();

		dirLocation = glGetUniformLocation(program, "dir");
		timeLocation = glGetUniformLocation(program, "time");
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(vertex_a);

	glUniform1f(dirLocation, rot);
	glUniform1f(timeLocation, wip_timeWindow());
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glClear(GL_DEPTH_BUFFER_BIT);
	return;
}

#undef UINT
#undef BUFF2

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

static int entIsItem(enum entity type) {
	switch(type) {
		case ENT_COBRA:
		case ENT_SNAKE:
			return 0;
		default:
			return 1;
	}
}

extern struct anim entity_anim[];

void drawEnts(unsigned int room, entity_t *ent, wip_globj_t pv) {
	static wip_obj_t object;
	for(int i = 0; i < ENT_MAX; ++i) {
		if(ent[i].type == ENT_NONE || ent[i].room != room) continue;
		wip_makeObject(&object);
		
		float angle = 0.0, oldAngle = 0.0;
		if(ent[i].d == 0 && entity_anim[i].d == 3)
			angle = TO_RAD(360.0);
		else if(ent[i].d == 3 && entity_anim[i].d == 0)
			oldAngle = TO_RAD(360.0);
		angle += TO_RAD(90.0 * ent[i].d);
		oldAngle += TO_RAD(90.0 * entity_anim[i].d);

		quat_rotate(
			object.rotation,
			wip_interpolate(angle, oldAngle, wip_eventPart(&entity_anim[i].rotateEvent, wip_easeInOut)),
			(float[]){0, 0, 1}
		);
		object.x = wip_interpolate(2 * ent[i].x, 2 * entity_anim[i].x, wip_eventPart(&entity_anim[i].moveEvent, wip_easeInOut));
		object.y = wip_interpolate(2 * ent[i].y, 2 * entity_anim[i].y, wip_eventPart(&entity_anim[i].moveEvent, wip_easeInOut));
		if(entIsItem(ent[i].type)) object.z = sin(wip_timeWindow() * 1.5f) * 0.025f;
		wip_glmdl_t *model = getEntModel(&ent[i]);
		drawModel(
			&object,
			model, pv, NULL
		);
	}
}

