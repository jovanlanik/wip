//
// WIP
// Copyright (c) 2020 Jovan Lanik
//

// Model data

const float model_vert[] = {
	 1.0f,	 1.0f,	 1.0f,
	-1.0f,	 1.0f,	 1.0f,
	-1.0f,	-1.0f,	 1.0f,
	 1.0f,	-1.0f,	 1.0f,

	 1.0f,	 1.0f,	-1.0f,
	-1.0f,	 1.0f,	-1.0f,
	-1.0f,	-1.0f,	-1.0f,
	 1.0f,	-1.0f,	-1.0f,
};

const unsigned char model_color[] = {
	0xff, 0x00, 0x00,
	0x00, 0xff, 0x00,
	0x00, 0x00, 0xff,
	0xff, 0xff, 0xff,

	0x00, 0x00, 0xff,
	0x00, 0xff, 0x00,
	0xff, 0x00, 0x00,
	0xff, 0xff, 0xff
};

const unsigned int model_indices[] = {
	0, 1, 3,
	1, 3, 2,
	3, 2, 7,
	2, 7, 6,
	1, 2, 6,
	6, 5, 1,
	3, 0, 4,
	7, 4, 3,
	7, 4, 6,
	6, 4, 5,
	5, 0, 4,
	5, 0, 1
};

