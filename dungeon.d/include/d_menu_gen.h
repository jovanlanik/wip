// Dungeon
// Copyright (c) 2012 Jovan Lanik

// Menu generator

#if defined MENU_NAME && defined MENU_HEADER && defined MENU_FUNC && defined MENU_LIST

#define _CAT(x, y) __CAT(x, y)
#define __CAT(x, y) x##y

void MENU_FUNC(unsigned int selected, void *p);

#define MENU_ITEM(x, y, z) x
enum _CAT(MENU_NAME, _item) { MENU_LIST, _CAT(MENU_NAME, _END) };
#undef MENU_ITEM

#define MENU_ITEM(x, y, z) { x, y, z }
menu MENU_NAME = {
	.header = MENU_HEADER,
	.func = MENU_FUNC,
	.item_c = _CAT(MENU_NAME, _END),
	.item = (menu_item[]){ MENU_LIST }
};
#undef MENU_ITEM

#undef __CAT
#undef _CAT

#undef MENU_NAME
#undef MENU_HEADER
#undef MENU_FUNC
#undef MENU_LIST

#endif
