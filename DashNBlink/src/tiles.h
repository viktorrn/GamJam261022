#pragma once

#include <vector>

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

struct room_change
{
	int prev_index;
	int x;
	int y;
};

struct room
{
	tile tiles[ROOM_DIMENSION][ROOM_DIMENSION];
	const char* map;
	const char* lookup;
	int index;
	std::vector<std::vector<room_change>> steps;
};

#define TILE_MAP_DIMENSION 16

extern void room_load(room* room, const char* map, const char* lookup, int index);

extern void room_load_next(room* room);

extern void room_draw(const room* room);

extern void room_remove_platform(room* room, float x, float y, bool start);

extern bool room_revert_last(room* room);

extern void room_revert_all(room* room);
