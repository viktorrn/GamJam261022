#pragma once


struct rect
{
	unsigned int vao;
	unsigned int p_vbo;
	unsigned int t_vbo;
	unsigned int ibo;
	int indices_count;
};

extern void rect_create(rect* rect);

extern void rect_draw(rect* rect);

extern void rect_destroy(rect* rect);