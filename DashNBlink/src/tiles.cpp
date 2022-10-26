#include "tiles.h"

#include <iostream>

#include "stb_image.h"

#include "graphics.h"

#define PI 3.141592

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

void room_load(room* room, const char* map, const char* lookup)
{
	stbi_set_flip_vertically_on_load(1);

	int map_width, map_height, bpp;
	unsigned char* map_buffer = stbi_load(map, &map_width, &map_height, &bpp, 3);

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
					
					vec2 v{1,0};
					vec2 v2{ 0,0 };
					vec2 v3{ 0,0 };
					rotateVec2(&v, &v2, angles[startAngle[y]%8]);

					rotateVec2(&v2, &v3, 2 * angles[x/4]);
					room->tiles[x][y].index = i;
					
					room->tiles[x][y].facing = v3;
					

					//std::cout << i << std::endl;
				}
			}
		}
	}

	stbi_image_free(lookup_buffer);

	stbi_image_free(map_buffer);
}

void room_draw(const room* room)
{
	for (int y = 0; y < ROOM_DIMENSION; y++)
	{
		for (int x = 0; x < ROOM_DIMENSION; x++)
		{
			graphics_tile_draw(&(room->tiles[x][y]), (float)x, (float)y);
		}
	}
}