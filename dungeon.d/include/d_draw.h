// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Drawing functions

#pragma once

#include "wip_mdl.h"
#include "wip_obj.h"
#include "wip_event.h"

#include "d_format.h"

struct anim {
	wip_event_t rotateEvent;
	wip_event_t moveEvent;
	enum direction d;
	union WIP_NAMED_VEC_T(2, int, WIP_XY, position, );
};

void drawModel(wip_obj_t *object, wip_glmdl_t *model, wip_globj_t pv, wip_obj_t *light);
void drawRoom(room_t *room, wip_globj_t pv);
void drawEnts(unsigned int room, entity_t *ent, wip_globj_t pv);

