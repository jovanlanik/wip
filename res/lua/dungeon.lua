local ffi = require("ffi")
ffi.cdef[[
const int getFilenameMax(void);
const int getEntMax(void);
void sendStaticToast(const char *toastMsg);
void sendMessage(const char *s);

enum direction {
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST
};
typedef struct {
	int x, y;
	enum direction d;
	int health;
} player_t;
enum entity {
	ENT_NONE,
	ENT_BOOK,
	ENT_KEY,
	ENT_HEAL,
	ENT_COBRA,
	ENT_SNAKE,
	ENT_LUA,
};
typedef struct {
	unsigned int room;
	enum entity type;
	int id;
	int x, y;
	enum direction d;
} entity_t;
]]

local filename_max = ffi.C.getFilenameMax()
local ent_max = ffi.C.getEntMax()

local state = [[
typedef struct {
	char dungeon[]] .. filename_max .. [[];
	unsigned int room;
	unsigned int keyring[9];
	entity_t entity[]] .. ent_max .. [[];
	player_t player;
} state_t;
state_t *getState(void);
]]
ffi.cdef(state)

local dungeon = {}
dungeon.sendToast = ffi.C.sendStaticToast
dungeon.sendMessage = ffi.C.sendMessage
dungeon.state = ffi.C.getState()

-- TODO: spawn and kill ent
return dungeon
