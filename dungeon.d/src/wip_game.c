// Dungeon
// Copyright (c) 2021 Jovan Lanik

// Rendering Functions

#define _POSIX_C_SOURCE 200809L

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
#include "wip_event.h"
#include "external/linmath.h"

#include "d_state.h"
#include "d_format.h"
#include "d_draw.h"
#include "d_text.h"
#include "d_menu.h"

extern const char _binary_dungeon_conf_start[];
extern const char _binary_d_texture_vert_start[];
extern const char _binary_d_texture_frag_start[];

// Engine internal
const char *wip_defaultConf = _binary_dungeon_conf_start;
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
wip_event_t cameraEvent;
enum direction oldDir;
wip_event_t rotateEvent;
wip_event_t moveEvent;
wip_event_t bumpEvent;
wip_event_t toastEvent;
struct { union WIP_NAMED_VEC_T(2, int, WIP_XY, position, ); } oldPos;

// Runtime
int started = 0;
int paused = 0;
char *message = NULL;
char *toast = NULL;
wip_obj_t center, camera;
wip_glmdl_t *sword_model;
wip_glmdl_t *snake_model;
dungeon_t d;
wip_globj_t projection;

// Game
state_t currentState;

static void initGameLoop(void) {
	wip_makeObject(&camera);
	camera.y = 0.0f;
	camera.z = 0.0f;

	wip_makeObject(&center);

	GLuint vertShader = wip_loadShader((char *)_binary_d_texture_vert_start, GL_VERTEX_SHADER);
	GLuint fragShader = wip_loadShader((char *)_binary_d_texture_frag_start, GL_FRAGMENT_SHADER);
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

void makeToast(char *toastMsg, float time) {
	wip_startEvent(&toastEvent, time);
	if(toast) wip_free(toast);
	toast = toastMsg;
}

static void newGame(void) {
	if(readDungeon(&d, &currentState,"./dungeon.d/example.df") != 0)
		wip_log(WIP_FATAL, "%s: Couldn't load dungeon from %s.", __func__, currentState.dungeon);

	oldPos.x = currentState.player.x;
	oldPos.y = currentState.player.y;
	oldDir = currentState.player.d;
	
	wip_startEvent(&cameraEvent, 0.5);
	wip_startEvent(&rotateEvent, 0.5);
	wip_startEvent(&moveEvent, 0.25);
	wip_startEvent(&bumpEvent, 0.125);
	makeToast(strdup("Welcome to the dungeon!"), 2.0);

	currentState.keyring[1] = 1;

	return;
}

static void action(void) {
	struct { union WIP_NAMED_VEC_T(2, int, WIP_XY, position, ); } target;
	target.x = currentState.player.x - abs((int)currentState.player.d - 1) + 1;
	target.y = currentState.player.y - abs((int)currentState.player.d - 2) + 1;
	room_t *room = &d.room[currentState.room];
	tile_t *tile = &room->tile[room->width*target.y+target.x];
	if(target.x == room->width || target.y == room->height) return;
	switch(tile->type) {
		case TILE_DOOR:
			if(tile->id == 0) break;
			else if(currentState.keyring[tile->id]) {
				makeToast(strdup("Unlocked door"), 2.0);
				room->deco[0][room->width*target.y+target.x].model = tile->data;
				tile->id = 0;
				return;
			}
			else {
				char *door_toast = strdup("This door is locked! You need key X");
				door_toast[strlen(door_toast)-1] = '0' + tile->id;
				makeToast(door_toast, 2.0);
			}
		case TILE_WALL:
			wip_startEvent(&bumpEvent, 0.125);
			return;
		case TILE_GATE:
			{
				char *gate_toast = strdup("Gate to room X");
				unsigned int id = currentState.room;
				currentState.room = tile->id;
				room_t *t = &d.room[tile->id];
				unsigned int w = t->width;
				struct { union WIP_NAMED_VEC_T(2, int, WIP_XY, position, ); } fail;
				for(int y = 0; y < t->height; ++y) {
					for(int x = 0; x < t->width; ++x) {
						if(t->tile[w*y+x].type == TILE_FLOOR) {
							fail.x = x;
							fail.y = y;
						}
						else if(t->tile[w*y+x].type == TILE_GATE && t->tile[w*y+x].id == id) {
							gate_toast[strlen(gate_toast)-1] = '0' + currentState.room;
							makeToast(gate_toast, 2.0);
							currentState.player.x = x;
							currentState.player.y = y;
							currentState.player.direction = t->deco[0][w*y+x].dir;
							return;
						}
					}
				}
				wip_debug(
					WIP_WARN,
					"%s: Couldn't find gate with id %u in room %u, using first floor tile.",
					__func__,
					id,
					currentState.room
				);
				currentState.player.x = fail.x;
				currentState.player.y = fail.y;
			}
		default:
			break;
	}
	wip_startEvent(&moveEvent, 0.25);
	oldPos.x = currentState.player.x;
	oldPos.y = currentState.player.y;
	currentState.player.x = target.x;
	currentState.player.y = target.y;
}

static void gameLoop(void) {
	if(wip_readMotion(HELP)) message =
		"# Help\n"
		"## Controls\n"
		"- WIP_UP = Forward/Attack/Use\n"
		"- WIP_LEFT = Turn left\n"
		"- WIP_RIGHT = Turn right\n"
		"- WIP_ENTER = Show inventory\n"
		"- 'H' = Show help message\n"
		"- WIP_ESC = Pause game";
	if(wip_readMotion(USE)) {
		char *msg = "# Inventory\n"
			"## Keyring\n"
			"[1]  2   3 \n"
			" 4   5   6 \n"
			" 7   8   9 ";
		makeToast(strdup(msg), 2.0);
	}
	if(!wip_eventRemainder(&moveEvent) && wip_readMotion(UP)) action();
	if(!wip_eventRemainder(&rotateEvent)) {
		if(wip_readMotion(RIGHT)) { 
			wip_startEvent(&rotateEvent, 0.5);
			oldDir = currentState.player.d;
			currentState.player.d--;
		}
		if(wip_readMotion(LEFT)) {
			wip_startEvent(&rotateEvent, 0.5);
			oldDir = currentState.player.d;
			currentState.player.d++;
		}
	}
	if(currentState.player.d < 0) currentState.player.d = 3;
	currentState.player.d %= 4;

	center.x = wip_interpolate(2*currentState.player.x, 2*oldPos.x, wip_eventPart(&moveEvent, wip_easeInOut));
	center.y = wip_interpolate(2*currentState.player.y, 2*oldPos.y, wip_eventPart(&moveEvent, wip_easeInOut));
	camera.x = 0;
	camera.y = -1;

	float angle = 0.0, oldAngle = 0.0;
	if(currentState.player.d == 0 && oldDir == 3)
		angle = TO_RAD(360.0);
	else if(currentState.player.d == 3 && oldDir == 0)
		oldAngle = TO_RAD(360.0);
	angle += TO_RAD(90.0 * (currentState.player.d+2));
	oldAngle += TO_RAD(90.0 * (oldDir+2));

	quat_rotate(camera.rotation,
		wip_interpolate(angle, oldAngle, wip_eventPart(&rotateEvent, wip_easeInOut)),
		(float[]){0, 0, 1}
	);

	vec3 camOld;
	quat_mul_vec3(camOld, camera.rotation, camera.position);
	vec3_add(camera.position, camOld, center.position);

	camera.z = wip_eventPart(&cameraEvent, wip_easeInOut)
		- wip_eventPart(&bumpEvent, wip_easeInOut)/25.0
		+ 1.0/25.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wip_globj_t pv, view;

	mat4x4_look_at(view.m, camera.position, center.position, axis);
	mat4x4_mul(pv.m, projection.m, view.m);

	// Render here...
	drawRoom(&d.room[currentState.room], pv);
	// Viewmodel
	glClear(GL_DEPTH_BUFFER_BIT);
	drawModel(&camera, sword_model, pv, NULL);
	// Interface
	glClear(GL_DEPTH_BUFFER_BIT);
	drawFormatStr(10, 10, 2.0f, "%4.1f", (startTime - lastTime) * 1000.0f);
	if(toast != NULL && wip_eventPart(&toastEvent, wip_easeLinear))
		drawStr(10, 20 + 2.0*CHAR_SIZE, 4.0f, toast);

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

static void mainMenuFn(unsigned int selected, void *p) {
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

static void pauseMenuFn(unsigned int selected, void *p) {
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

static void m_menuLoop(menu *menu) {
	static int selected = 0;
	if(wip_readMotion(DOWN)) selected++;
	if(wip_readMotion(UP)) selected--;
	selected = (selected + menu->item_c) % menu->item_c;
	if(wip_readMotion(USE)) {
		menu->func(selected, NULL);
		selected = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMenu(*menu, selected, NULL);
}

static void p_menuLoop(menu *menu) {
	message = NULL;

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

static void messageLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(started && !paused) {
		wip_globalKeyLock = 1;
		gameLoop();
		wip_globalKeyLock = 0;
	}
	drawStr(10, 10, 4, message);

	if(wip_readMotion(USE) || wip_readMotion(HELP)) message = NULL;
}

void wip_gameLoop(void) {
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);

	initGameLoop();

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
			else if(message != NULL) messageLoop();
			else gameLoop();
		} else m_menuLoop(&mainMenu);

		wip_pollWindow();
		wip_swapWindow();
	}
	return;
}

