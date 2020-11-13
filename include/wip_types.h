//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Type Generator Macros

#pragma once

// TODO: Add WIP prefix to macros

#define XYZ(type) { type x, y, z; }
#define RGB(type) { type r, g, b; }
#define RGBA(type) { type r, g, b, a; }

#define NAMED_VEC(size, type, structure, name, vec) { type name[size]; struct structure(type) vec; }
#define NAMED_MAT(w, h, type, name, mat) { type name[(w*h)]; type mat[w][h]; }

#define POS NAMED_VEC(3, float, XYZ, position, )

#define FIFO(size, type) { int head, tail; type buffer[size]; }

