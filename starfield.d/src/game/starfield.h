// Starfield
// Copyright (c) 2020 Jovan Lanik

// Starfield card game

//#include <stdio.h>

// TODO: change ints to more specific types (uint8 and such)

enum faction {
	EARTH,
	MARS
};

typedef void fn_t(int x, int y, enum faction);

typedef union {
	int all[3];
	struct {
		int scrap;
		int tech;
		int personel;
	};
} resource_t;

typedef struct {
	char *name;
	resource_t cost;
	resource_t material;
	int capacity;
	struct {
		fn_t *start;
		fn_t *end;
		fn_t *place;
		fn_t *exhaust;
	} effect;
	struct {
		int energy;
		resource_t cost;
		fn_t * function;
	} *action;
} card_t;

typedef struct {
	card_t *field[6][4];
	card_t *hand[5];
} faction_t;

typedef faction_t state_t[2];

