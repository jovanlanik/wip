// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Player

#pragma once

#include "wip_types.h"
#include "dungeon.h"

typedef struct {
	union WIP_NAMED_VEC_T(2, int, WIP_XY, position, );
	union { enum direction d; enum direction direction; };
} player_t;
