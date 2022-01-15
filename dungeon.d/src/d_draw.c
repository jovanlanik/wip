// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Drawing functions

#include "wip_mdl.h"
#include "wip_obj.h"
#include "wip_img.h"
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
	if(!init) {
		init = 1;
		texture = wip_openTexture("d_wall");
	}

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
	for(int i = 0; i < room->width; ++i) {
		for(int n = 0; n < room->height; ++n) {
			wip_obj_t obj;
			wip_makeObject(&obj);
			obj.x = 2*i - 10;
			obj.y = 2*n - 10;
			if(*room->deco[i][n] == DECO_MODEL)
				drawModel(&obj, ((struct deco_model*)room->deco[i][n])->model, pv, NULL);
		}
	}
	return;
}


