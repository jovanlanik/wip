// WIP
// Copyright (c) 2021 Jovan Lanik

// Event Functions

#include "wip_event.h"

wip_event_t *wip_startEvent(wip_event_t *event, double len) {
	event->length = len;
	event->start = wip_timeWindow();
	return event;
}

