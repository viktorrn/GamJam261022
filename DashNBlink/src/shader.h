#pragma once

struct shader
{
	unsigned int id;
};

extern unsigned int shader_compile(const char* source, unsigned int type);

extern void shader_create(shader* s, const char* path);

extern void shader_bind(const shader* s);

extern void shader_unbind();

extern unsigned int shader_find_location(shader* s, const char* name);

extern void shader_set_uniform1f(unsigned int location, float x);

extern void shader_set_uniform2f(unsigned int location, float x, float y);

extern void shader_set_uniform3f(unsigned int location, float x, float y, float z);

extern void shader_destroy(shader* s);
