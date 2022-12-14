#pragma once

#include <vector>

#include "miniaudio.h"
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

	std::vector<vec2> steps;
};

extern void player_load(player* p, const room* r, int character);

extern void player_tick(player* p, float deltaT, room* r, ma_engine*engine);

extern void player_draw(const player* p);