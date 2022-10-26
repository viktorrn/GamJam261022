#pragma once

#include "tiles.h"
#include "struct.h"

struct player
{
	vec2 position{};
	vec2 target{};
	vec2 oldPosition{};
	bool moving;
	float rotation;
	int character;
	float flightTime;
};

extern void player_load(player* p, const room* r, int character);

extern void player_tick(player* p, float deltaT);

extern void player_draw(const player* p);