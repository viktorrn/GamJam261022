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
				p->position.x = 0;
				p->position.y = 0;

				p->character = 240 + character;
				p->rotation = 0.0f;
				p->moving = false;

				p->target.x = 15;//(float)x;
				p->target.y = 15;//(float)y;

				p->oldPosition.x = 0;//(float)x;
				p->oldPosition.y = 0;//(float)y;
				p->flightTime = 0;
				std::cout << "Player created at: [" << x << ", " << y << "]" << std::endl;

				return;
			}
		}
	}

	p->position.x = 1.0f;
	p->position.y = 1.0f;
	p->character = 240 + character;
	p->rotation = 0.0f;

	std::cout << "Player entry point not found, created at: [1, 1]" << std::endl;
}

void player_tick(player* p, float deltaT)
{
	float px(0); float py(0);
	float duration = 1.0f;
	
	p->flightTime += deltaT;
	
	float t = clip((1 - p->flightTime /duration),0,1);
	float tp = ((t * t) / (2 * (t * t - t) + 1));
	
	p->position.x = p->target.x + ((p->oldPosition.x- p->target.x) * tp); //p->oldPosition.x * ((t * t) / (2 * (t * t - t) + 1));
	p->position.y = p->target.y + ((p->oldPosition.y - p->target.y) * tp); //p->oldPosition.y * ((t * t) / (2 * (t * t - t) + 1));

	p->rotation += 0.03f;
	

}

void player_draw(const player* p)
{
	graphics_player_draw(p);
}