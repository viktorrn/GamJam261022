#include "player.h"

#include <iostream>

#include "graphics.h"

void player_load(player* p, const room* r, int character)
{
	for (int y = 0; y < ROOM_DIMENSION; y++)
	{
		for (int x = 0; x < ROOM_DIMENSION; x++)
		{
			if (r->tiles[x][y].index == 14)
			{
				p->x = (float)x;
				p->y = (float)y;
				p->character = 240 + character;
				p->rotation = 0.0f;

				std::cout << "Player created at: [" << x << ", " << y << "]" << std::endl;

				return;
			}
		}
	}

	p->x = 1.0f;
	p->y = 1.0f;
	p->character = 240 + character;
	p->rotation = 0.0f;

	std::cout << "Player entry point not found, created at: [1, 1]" << std::endl;
}

void player_tick(player* p)
{
	p->x += 0.02f;
	p->rotation += 0.03f;
}

void player_draw(const player* p)
{
	graphics_player_draw(p);
}