//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Type Generator Macros

#pragma once

#define WIP_XYZ(type) { type x, y, z; }
#define WIP_RGB(type) { type r, g, b; }
#define WIP_RGBA(type) { type r, g, b, a; }

#define WIP_NAMED_VEC(size, type, structure, name, vec) { type name[size]; struct structure(type) vec; }
#define WIP_NAMED_MAT(w, h, type, name, mat) { type name[(w*h)]; type mat[w][h]; }

#define WIP_POS WIP_NAMED_VEC(3, float, WIP_XYZ, position, )

#define WIP_FIFO(size, type) { int head, tail; type buffer[size]; }

