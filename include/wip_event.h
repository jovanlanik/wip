// WIP
// Copyright (c) 2021 Jovan Lanik

// Event Functions

#pragma once

#include "wip_window.h"

typedef double (*wip_easing_t)(double val);

typedef struct {
	double length;
	double start;
} wip_event_t;

wip_event_t *wip_startEvent(wip_event_t *event, double len);

static inline double wip_interpolate(double start, double end, double part) {
	if(part <= 0.0) return start;
	if(part >= 1.0) return end;
	return part * (end - start) + start;
}

static inline double wip_easeLinear(double val) { return val; }
static inline double wip_easeIn(double val) { return val * val; }
static inline double wip_easeOut(double val) { return 1 - wip_easeIn(1 - val); }
static inline double wip_easeInOut(double val) { return wip_interpolate(wip_easeIn(val), wip_easeOut(val), val); }

static inline double wip_eventRemainder(wip_event_t *event) {
	double val =  event->length + event->start - wip_timeWindow();
	return val < 0.0 ? 0.0 : val;
}

static inline double wip_eventPart(wip_event_t *event, wip_easing_t fn) {
	return fn(wip_eventRemainder(event)/event->length);
}
