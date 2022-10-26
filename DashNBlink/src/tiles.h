#pragma once

#include "texture.h"

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
};

#define ROOM_DIMENSION 16

struct room
{
	tile tiles[ROOM_DIMENSION][ROOM_DIMENSION];
};

#define TILE_MAP_DIMENSION 16

extern void room_load(room* room, const char* map, const char* lookup);

extern void room_draw(const room* room);
