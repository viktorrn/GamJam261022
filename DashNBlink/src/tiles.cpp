#include "tiles.h"

#include <iostream>
#include <sstream>

#include "stb_image.h"

#include "graphics.h"

#define PI 3.141592

using namespace std;

void sprite_sheet_create(sprite_sheet* sheet, const char* sprites)
{
	stbi_set_flip_vertically_on_load(1);

	int sprites_width, sprites_height, bpp;
	unsigned char* sprites_buffer = stbi_load(sprites, &sprites_width, &sprites_height, &bpp, 4);

	if (sprites_buffer == nullptr)
	{
		std::cout << "Failed to read image path: " << sprites << std::endl;
		return;
	}

	texture_create(&(sheet->tex), sprites_buffer, sprites_width, sprites_height);

	stbi_image_free(sprites_buffer);
}

extern void sprite_sheet_destroy(sprite_sheet* sheet)
{
	texture_destroy(&(sheet->tex));
}

void room_load(room* room, const char* map, const char* lookup, int index)
{
	std::stringstream map_path;
	map_path << map;
	map_path << index;
	map_path << ".png";

	stbi_set_flip_vertically_on_load(1);

	int map_width, map_height, bpp;
	unsigned char* map_buffer = stbi_load(map_path.str().c_str(), &map_width, &map_height, &bpp, 3);

	if (map_buffer == nullptr)
	{
		std::cout << "Failed to read image path: " << map << std::endl;
		return;
	}

	int lookup_width, lookup_height;
	unsigned char* lookup_buffer = stbi_load(lookup, &lookup_width, &lookup_height, &bpp, 3);

	if (lookup_buffer == nullptr)
	{
		std::cout << "Failed to read image path: " << lookup << std::endl;
		return;
	}

	int startAngle[16] =
	{
		0, 0, 0, 0, 0, 0, 0 ,0,
		4, 4, 1, 1, 1, 7, 0, 0, 
	};

	float angles[8] =
	{
		0,PI/4,PI/2,3*PI/4,PI,5*PI/4,6*PI/4,7*PI/4
	};


	for (int y = 0; y < map_height; y++)
	{
		for (int x = 0; x < map_width; x++)
		{
			for (int i = lookup_width * lookup_height - 1; i > -1; i--)
			{
				if (lookup_buffer[i * 3] == map_buffer[(y * map_width + x) * 3] &&
					lookup_buffer[i * 3 + 1] == map_buffer[(y * map_width + x) * 3 + 1] && 
					lookup_buffer[i * 3 + 2] == map_buffer[(y * map_width + x) * 3 + 2])
				{
					int lookup_x = i % 16;
					int lookup_y = i / 16;


					//std::cout << ""


					vec2 v{ 1,0 };

					if (lookup_y == 0)
					{
						v.x = 0;
					}

					vec2 v2{ 0,0 };
					rotateVec2(&v, &v2, angles[startAngle[lookup_y] % 8]);

					vec2 v3{ 0,0 };
					rotateVec2(&v2, &v3, 2 * angles[lookup_x /4]);
 

					room->tiles[x][y].index = i;
					
					room->tiles[x][y].facing = v3;
				}
			}
		}
	}

	stbi_image_free(lookup_buffer);

	stbi_image_free(map_buffer);

	room->map = map;
	room->lookup = lookup;
	room->index = index;
}

void room_load_next(room* room)
{
	room->steps = std::vector<std::vector<room_change>>();
	int new_index = room->index + 1;
	const char* map = room->map;
	const char* lookup = room->lookup;
	room_load(room, map, lookup, new_index);
}

void room_tick(room* room, float delta)
{
	if (room->shaking_x)
	{
		if (room->shake_x < 2.0f * 3.1415f)
		{
			room->shake_x += 0.8f;
		}

		else
		{
			room->shake_x = 0.0f;
			room->shaking_x = false;
		}
	}

	if (room->shaking_y)
	{
		if (room->shake_y < 2.0f * 3.1415f)
		{
			room->shake_y += 0.8f;
		}

		else
		{
			room->shake_y = 0.0f;
			room->shaking_y = false;
		}
	}
}

void room_draw(const room* room)
{
	for (int y = 0; y < ROOM_DIMENSION; y++)
	{
		for (int x = 0; x < ROOM_DIMENSION; x++)
		{
			graphics_tile_draw(&(room->tiles[x][y]), (float)x + sinf(room->shake_x) * 0.15f, (float)y + sinf(room->shake_y) * 0.15f);
		}
	}
}

void room_remove_platform(room* room, float x, float y, bool start)
{
	if (start)
	{
		room->steps.push_back(std::vector<room_change>());
	}

	int center_x = (int)(x + 0.5f);
	int center_y = (int)(y + 0.5f);

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (room->tiles[center_x + i][center_y + j].index != 12)
			{
				room_change ch;
				ch.prev_index = room->tiles[center_x + i][center_y + j].index;
				ch.x = center_x + i;
				ch.y = center_y + j;
				room->steps[room->steps.size() - 1].push_back(ch);

				room->tiles[center_x + i][center_y + j].index = 12;

				room_remove_platform(room, (float)(center_x + i), (float)(center_y + j), false);
			}
		}
	}
}

bool room_revert_last(room* room)
{
	if (room->steps.size() != 0)
	{
		for (int i = 0; i < room->steps[room->steps.size() - 1].size(); i++)
		{
			room_change ch = room->steps[room->steps.size() - 1][i];
			room->tiles[ch.x][ch.y].index = ch.prev_index;
		}

		room->steps.pop_back();

		return true;
	}

	return false;
}

void room_revert_all(room* room)
{
	bool removed = room_revert_last(room);

	while (removed)
	{
		removed = room_revert_last(room);
	}
}

void room_shake_x(room* room)
{
	room->shaking_x = true;
}

void room_shake_y(room* room)
{
	room->shaking_y = true;
}