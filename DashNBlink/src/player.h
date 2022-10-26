#pragma once

#include "tiles.h"

struct player
{
	float x;
	float y;
	float rotation;
	int character;
};

extern void player_load(player* p, const room* r, int character);

extern void player_tick(player* p);

extern void player_draw(const player* p);