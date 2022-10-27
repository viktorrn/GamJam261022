#include "player.h"

#include <iostream>
#include "keyboard.h"
#include "graphics.h"
#include "tiles.h"
#define PI 3.141592

using namespace std;

void player_load(player* p, const room* r, int character)
{
	int entryIndex_x = 4;
	int entryIndex_y = 4;
	int startIndex_x = 8;
	int startIndex_y = 8;
	for (int y = 0; y < ROOM_DIMENSION; y++)
	{
		for (int x = 0; x < ROOM_DIMENSION; x++)
		{
			if (r->tiles[x][y].index == 14)
			{
				startIndex_x = x;
				startIndex_y = y;
			}
			if (r->tiles[x][y].index == 11)
			{
				entryIndex_x = x;
				entryIndex_y = y;
			}
		}
	}

	p->position.x = 0;
	p->position.y = 0;

	p->character = 240 + character;
	p->rotation = 0.0f;
	p->moving = true;

	p->target.x = (float)startIndex_x;//
	p->target.y = (float)startIndex_y;//(float)y;

	p->oldPosition.x = (float)entryIndex_x;
	p->oldPosition.y = (float)entryIndex_y;

	p->moveDirection.x = startIndex_x - entryIndex_x;
	p->moveDirection.x = startIndex_y - entryIndex_y;

	p->flightTime = 0;
	std::cout << "Player created at: [" << entryIndex_x << ", " << entryIndex_y << "]" << std::endl;
	p->position.x = 1.0f;
	p->position.y = 1.0f;
	p->character = 240 + character;
	p->rotation = 0.0f;
	p->dead = false;
	p->done = false;
	p->steps = std::vector<vec2>();
}

bool checkTileContentsEmpty(vec2* vec, const room* r);
bool checkInBounds(vec2* vec);
tile getTileAt(vec2* vec, const room* r);

void player_tick(player* p, float deltaT, room* r)
{
	float px(0); float py(0);
	float duration = 0.35f;

	if (p->moving)
	{
		p->flightTime += deltaT;

		float t = clip((1 - p->flightTime / duration), 0, 1);
		float tp = ((t * t) / (2 * (t * t - t) + 1));
		float tp2 = 8*(-(t*t*t) / 3 + (t * t)/2 -0.09*t);

		float t2 = clip((1 - p->flightTime / duration*1.3), 0, 1);
		float tr = ((t2 * t2) / (2 * (t2 * t2 - t2) + 1));
		

		p->position.x = p->target.x + ((p->oldPosition.x - p->target.x) * tp);
		p->position.y = p->target.y + ((p->oldPosition.y - p->target.y) * tp);

		if(abs(p->moveDirection.x)) 
			p->rotation = 2 * PI * tr * -p->moveDirection.x;
		else if(abs(p->moveDirection.y)) 
			p->rotation = 2 * PI * tr * -p->moveDirection.y;
			

		if (abs(p->position.x - p->target.x) < 0.001 && abs(p->position.y - p->target.y) < 0.001)
		{
			p->moving = false;
			p->flightTime = 0;
			p->oldPosition = p->target;

			int index = getTileAt(&(p->position), r).index;
			
			if (index == 15)
			{
				std::cout << "Win!!!" << std::endl;
				p->done = true;
			}
			
			else if (index != 12 && index != 14)
			{
				p->dead = true;
			}
		}
	}

	else if (keyboard_is_pressed(GLFW_KEY_SPACE))
	{
		if (p->steps.size() != 0)
		{
			p->moving = true;
			p->target = p->steps[p->steps.size() - 1];
			p->steps.pop_back();
		}

		room_revert_last(r);
	}

	else
	{
		vec2 moveDirection{0,0};
		if (keyboard_is_pressed(GLFW_KEY_RIGHT))
		{
			moveDirection = {1,0};
			
		}

		else if (keyboard_is_pressed(GLFW_KEY_LEFT))
		{
			moveDirection = {-1,0};
			
		}
		else if (keyboard_is_pressed(GLFW_KEY_UP))
		{
			moveDirection = {0,1};
			
		}
		else if (keyboard_is_pressed(GLFW_KEY_DOWN))
		{
			moveDirection = {0,-1};
			
		}

		if (moveDirection.x == 0 && moveDirection.y == 0) return;
		p->moving = true;

		//std::cout << "move requested " << moveDirection.x << " , " << moveDirection.y << std::endl;

		vec2 calcPosition = p->position;

		while (true)
		{
			//std::cout << "move " << calcPosition.x << " , " << calcPosition.y;
			//std::cout << std::endl;
			addVec2(&calcPosition, &moveDirection);
			

			if (!checkTileContentsEmpty(&calcPosition, r))
			{
				tile tile = getTileAt(&calcPosition,r);
				//cout << "tile facing data " << tile.facing.x << " , " << tile.facing.y << endl;

				if (calcDotProductVec2(moveDirection, tile.facing) < 0)
				{
					subVec2(&calcPosition, &moveDirection);
				}

				p->steps.push_back(p->position);
				room_remove_platform(r, p->position.x, p->position.y, true);
				p->target = calcPosition;
				p->moveDirection = moveDirection;
				break;
			}

			if (!checkInBounds(&calcPosition))
			{
				p->target = p->position;
				break;

			}
		}

	}

	//
}

bool checkTileContentsEmpty(vec2 *vec, const room* r)
{
	if (r->tiles[(int)(vec->x+0.5f)][(int)(vec->y+0.5f)].index == 12 || r->tiles[(int)(vec->x + 0.5f)][(int)(vec->y + 0.5f)].index == 14) return true;
	return false;
}

tile getTileAt(vec2* vec, const room* r)
{
	return r->tiles[(int)(vec->x + 0.5f)][(int)(vec->y + 0.5f)];
}

bool checkInBounds(vec2* vec)
{
	if (vec->x >= 0 && vec->x < ROOM_DIMENSION && vec->y >= 0 && vec->y < ROOM_DIMENSION) return true;
	return false;
}

void player_draw(const player* p)
{
	graphics_player_draw(p);
}