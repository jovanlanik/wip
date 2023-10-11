// WFDD
// Copyright (c) 2023 Jovan Lanik

// WFDD for C (not C++)

#include "wfdd.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

wfdd_display_t *wfdd_initDisplay(const char *mcastgroup, int port) {
	wfdd_display_t *display = malloc(sizeof(wfdd_display_t));

	display->socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(display->socket == -1) {
		free(display);
		return NULL;
	}

	unsigned char ttl = 1;
	if(setsockopt(display->socket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) != 0) {
		close(display->socket);
		free(display);
		return NULL;
	}

	memset(&display->addr, 0, sizeof(display->addr));
	display->addr.sin_port = htons(port);
	display->addr.sin_family = AF_INET;
	if(inet_pton(AF_INET, mcastgroup, &display->addr.sin_addr) != 1) {
		close(display->socket);
		free(display);
		return NULL;
	}

	return display;
}

int wfdd_sendBuffer(wfdd_display_t *display, size_t size, uint8_t *data) {
	return sendto(display->socket, data, size, 0, (struct sockaddr *)&display->addr, sizeof(display->addr));
}

void wfdd_closeDisplay(wfdd_display_t *display) {
	close(display->socket);
}

