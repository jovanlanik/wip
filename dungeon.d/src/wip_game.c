// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Rendering Functions

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "wip_fn.h"
#include "wip_conf.h"
#include "wip_img.h"
#include "wip_obj.h"
#include "wip_mdl.h"
#include "wip_game.h"
#include "wip_window.h"
#include "wip_input.h"
#include "wip_gl.h"
#include "wip_math.h"
#include "external/linmath.h"

#include "d_state.h"
#include "d_format.h"
#include "d_draw.h"
#include "d_text.h"
#include "d_menu.h"

#include "include/baked/dungeon_config.h"
#include "include/baked/shaders.h"

// Engine internal
const char *wip_defaultConf = dungeon_conf;
int wip_globalTicksPerSecond;
// Engine external
extern int wip_globalKeyLock;
extern wip_window_t wip_globalWindow;

// Rendering
const float m[3] = { -1.5, 2.0, 0.4 };
vec3 axis = {0.0f, 0.0f, 1.0f};
GLuint program;
unsigned int lightLocation;
unsigned int mpvLocation;
unsigned int transformLocation;
unsigned int normalTransformLocation;
unsigned int materialLocation;
double startTime, lastTime;

// Runtime
int started = 0;
int paused = 0;
wip_obj_t center, camera;
wip_glmdl_t *sword_model;
wip_glmdl_t *snake_model;
room_t *currentRoom = NULL;
//dungeon_t d;
wip_globj_t projection;

// Game
state_t currentState;

void initGameLoop(void) {
	wip_makeObject(&camera);
	camera.y = 0.0f;
	camera.z = 0.0f;

	wip_makeObject(&center);

	GLuint vertShader = wip_loadShader((char *)d_texture_vert, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char *)d_texture_frag, GL_FRAGMENT_SHADER);
	program = wip_loadProgram(vertShader, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	sword_model = wip_openModel("d_sword");
	snake_model = wip_openModel("d_snake");

	lightLocation = glGetUniformLocation(program, "light");
	mpvLocation = glGetUniformLocation(program, "mpv");
	transformLocation = glGetUniformLocation(program, "transform");
	normalTransformLocation = glGetUniformLocation(program, "normalTransform");
	materialLocation = glGetUniformLocation(program, "material");

	float ratio = (float)wip_getConfInt("video.width")/(float)wip_getConfInt("video.height");
	mat4x4_perspective(projection.m, TO_RAD(wip_getConfFloat("game.fov")), ratio, 0.1, 1000);

	return;
}

void newGame(void) {
	currentState.player.x = 0;
	currentState.player.y = 0;
	currentState.player.d = DIR_NORTH;

	if(currentRoom) wip_free(currentRoom);
	currentRoom = testRoom();
	//readDungeon(&d, "./dungeon.d/example.df");

	return;
}

void gameLoop(void) {
	if(wip_readMotion(UP)) {
		currentState.player.x += abs((int)currentState.player.d%4-2)-1;
		currentState.player.y += abs((int)currentState.player.d+1%4-2)-1;
	}
	if(wip_readMotion(DOWN)) {
		currentState.player.x -= abs((int)currentState.player.d%4-2)-1;
		currentState.player.y -= abs((int)currentState.player.d+1%4-2)-1;
	}
	if(wip_readMotion(RIGHT)) currentState.player.d++;
	if(wip_readMotion(LEFT)) currentState.player.d--;
	currentState.player.d %= 4;

	center.x = 2*currentState.player.x;
	center.y = 2*currentState.player.y;
	camera.x = center.x - abs((int)currentState.player.d%4-2)+1;
	camera.y = center.y - abs((int)currentState.player.d+1%4-2)+1;

	quat_rotate(camera.rotation, -TO_RAD(90*(currentState.player.d+1)), (float[]){0, 0, 1});

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wip_globj_t pv, view;

	mat4x4_look_at(view.m, camera.position, center.position, axis);
	mat4x4_mul(pv.m, projection.m, view.m);

	// Render here...
	drawRoom(currentRoom, pv);
	// Viewmodel
	glClear(GL_DEPTH_BUFFER_BIT);
	drawModel(&camera, sword_model, pv, NULL);
	// Interface
	glClear(GL_DEPTH_BUFFER_BIT);
	drawFormatStr(10, 10, 2.0f, "%4.1f", (startTime - lastTime) * 1000.0f);

	//wip_glError();
	return;
}

#define MENU_NAME mainMenu
#define MENU_HEADER "Dungeon"
#define MENU_FUNC mainMenuFn
#define MENU_LIST \
	MENU_ITEM(M_NEW_GAME, "New Game", "Start a new game"), \
	MENU_ITEM(M_LOAD_GAME, "Load Game", "Load a saved game"), \
	MENU_ITEM(M_QUIT_GAME, "Quit Game", NULL)
#include "d_menu_gen.h"
#define MENU_NAME pauseMenu
#define MENU_HEADER "Paused"
#define MENU_FUNC pauseMenuFn
#define MENU_LIST \
	MENU_ITEM(P_CONTINUE, "Continue", "Continue current game"), \
	MENU_ITEM(P_NEW_GAME, "New Game", "Start a new game"), \
	MENU_ITEM(P_LOAD_GAME, "Load Game", "Load a saved game"), \
	MENU_ITEM(P_QUIT_GAME, "Quit Game", NULL)
#include "d_menu_gen.h"

void mainMenuFn(unsigned int selected, void *p) {
	switch(selected) {
		case M_NEW_GAME:
			newGame();
			started = 1;
			break;
		case M_QUIT_GAME:
			wip_globalWindow.close = 1;
		default:
			break;
	}
}

void pauseMenuFn(unsigned int selected, void *p) {
	switch(selected) {
		case P_NEW_GAME:
			newGame();
		case P_CONTINUE:
			paused = 0;
			break;
		case P_QUIT_GAME:
			wip_globalWindow.close = 1;
		default:
			break;
	}
}

void m_menuLoop(menu *menu) {
	static int selected = 0;
	if(wip_readMotion(DOWN)) selected++;
	if(wip_readMotion(UP)) selected--;
	selected = (selected + menu->item_c) % menu->item_c;
	if(wip_readMotion(USE)) {
		menu->func(selected, NULL);
		selected = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	wip_sleep(0.01);
	drawMenu(*menu, selected, NULL);
}

void p_menuLoop(menu *menu) {
	static int selected = 0;
	if(wip_readMotion(DOWN)) selected++;
	if(wip_readMotion(UP)) selected--;
	selected = (selected + menu->item_c) % menu->item_c;
	if(wip_readMotion(USE)) {
		menu->func(selected, NULL);
		selected = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	wip_globalKeyLock = 1;
	gameLoop();
	wip_globalKeyLock = 0;

	drawMenu(*menu, selected, NULL);
}

void wip_renderThread(void) {
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);

	initGameLoop();

	int fpsMax = wip_getConfInt("video.fpsMax");
	lastTime = wip_timeWindow();

	while(!wip_globalWindow.close) {
		lastTime = startTime;
		startTime = wip_timeWindow();

		wip_key_t key;
		while((key = wip_readKey()).action) {
			if(key.key == WIP_LOCKED) break;
			if(key.action == WIP_RELEASE && key.key == WIP_ESC) paused = 1;
			wip_writeMotion(key);
		}

		if(started) {
			if(paused) p_menuLoop(&pauseMenu);
			else gameLoop();
		} else m_menuLoop(&mainMenu);

		wip_pollWindow();
		do wip_swapWindow();
		while(wip_timeWindow() - startTime < 1.0/fpsMax && fpsMax && !wip_globalWindow.close);
	}
	return;
}

