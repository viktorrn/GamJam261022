#pragma once

#define SPRITE_DIMENSION 4
#define SPRITE_SHEET_DIMENSION 16

struct sprite_sheet
{
	float data[SPRITE_SHEET_DIMENSION * SPRITE_DIMENSION * SPRITE_SHEET_DIMENSION * SPRITE_DIMENSION * 4];
};

void sprite_sheet_load(sprite_sheet* sheet, const char* sprites);

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

void room_load(room* room, const char* map, const char* lookup);
