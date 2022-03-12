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

double wip_interpolate(double start, double end, double part);

double wip_easeLinear(double val);
double wip_easeIn(double val);
double wip_easeOut(double val);
double wip_easeInOut(double val);

double wip_eventRemainder(wip_event_t *event);
double wip_eventPart(wip_event_t *event, wip_easing_t fn);

