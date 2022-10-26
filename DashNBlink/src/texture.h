#pragma once

struct texture
{
	unsigned int id;
};

void texture_create(texture* tex, unsigned char* data, int width, int height);

void texture_bind(const texture* tex, int slot);

void texture_unbind(int slot);

void texture_destroy(texture* tex);