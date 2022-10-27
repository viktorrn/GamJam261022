#pragma once

#include "tiles.h"
#include "struct.h"

struct player
{
	vec2 target{};
	vec2 position{};
	vec2 oldPosition{};
	vec2 moveDirection{};
	
	bool moving;
	float rotation;
	int character;
	float flightTime;
	bool dead;
	bool done;
};

extern void player_load(player* p, const room* r, int character);

extern void player_tick(player* p, float deltaT, const room* r);

extern void player_draw(const player* p);