// WFDD
// Copyright (c) 2023 Jovan Lanik

// WFDD for C (not C++)

#include <stddef.h>
#include <stdint.h>
#include <netinet/ip.h>

typedef struct {
	int socket;
	struct sockaddr_in addr;
} wfdd_display_t;

wfdd_display_t *wfdd_initDisplay(const char *mcastgroup, int port);
int wfdd_sendBuffer(wfdd_display_t *display, size_t size, uint8_t *data);
void wfdd_closeDisplay(wfdd_display_t *display);

