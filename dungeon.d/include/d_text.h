// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Text drawing

#pragma once

#define CHAR_SIZE 26

void drawChar(char c, unsigned int x, unsigned int y, unsigned int ox, unsigned int oy, float scale);
void drawStr(unsigned int x, unsigned int y, float scale, char *str);
void drawFormatStr(unsigned int x, unsigned int y, float scale, char *format, ...);

