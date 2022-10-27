#pragma once

#include "texture.h"
#include "struct.h"

#define SPRITE_DIMENSION 4
#define SPRITE_SHEET_DIMENSION 16

struct sprite_sheet
{
	texture tex;
};

extern void sprite_sheet_create(sprite_sheet* sheet, const char* sprites);
extern void sprite_sheet_destroy(sprite_sheet* sheet);

struct tile
{
	int index;
	vec2 facing{0,0};
};

#define ROOM_DIMENSION 16

struct room
{
	tile tiles[ROOM_DIMENSION][ROOM_DIMENSION];
	const char* map;
	const char* lookup;
	int index;
};

#define TILE_MAP_DIMENSION 16

extern void room_load(room* room, const char* map, const char* lookup, int index);

extern void room_load_next(room* room);

extern void room_draw(const room* room);
