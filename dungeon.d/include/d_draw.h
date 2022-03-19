// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Drawing functions

#pragma once

#include "wip_mdl.h"
#include "wip_obj.h"

#include "d_format.h"

void drawModel(wip_obj_t *object, wip_glmdl_t *model, wip_globj_t pv, wip_obj_t *light);
void drawRoom(room_t *room, wip_globj_t pv);
void drawEnts(entity_t *ent, wip_globj_t pv);

