// WIP
// Copyright (c) 2021 Jovan Lanik

// Event Functions

#include "wip_event.h"

wip_event_t *wip_startEvent(wip_event_t *event, double len) {
	event->length = len;
	event->start = wip_timeWindow();
	return event;
}

double wip_interpolate(double start, double end, double part) {
	return part * (end - start) + start;
}

double wip_easeLinear(double val) { return val; }
double wip_easeIn(double val) { return val * val; }
double wip_easeOut(double val) { return 1 - wip_easeIn(1 - val); }
double wip_easeInOut(double val) { return wip_interpolate(wip_easeIn(val), wip_easeOut(val), val); }

double wip_eventRemainder(wip_event_t *event) {
	double val = event->length + event->start - wip_timeWindow();
	return val < 0.0 ? 0.0 : val;
}

double wip_eventPart(wip_event_t *event, wip_easing_t fn) {
	if(event->length == 0.0) return 0.0;
	else return fn(wip_eventRemainder(event)/event->length);
}

