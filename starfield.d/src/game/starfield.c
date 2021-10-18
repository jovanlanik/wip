// Starfield
// Copyright (c) 2021 Jovan Lanik

// Starfield card game

#include <stdio.h>

#include "starfield.h"

int main(void) {
	card_t station = { .name = "G-Ring Station" };
	station.cost.tech = 3;

	state_t state;
	state[EARTH].field[0][0] = &station;

	printf("%s\n", state[EARTH].field[0][0]->name);

	return 0;
}
