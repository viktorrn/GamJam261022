#pragma once
#include <utility>

struct vec2
{
	float x, y;
};

inline void addVec2(vec2 *v1, vec2 *v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
}

inline void subVec2(vec2* v1, vec2* v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
}

inline void rotateVec2(vec2* v_i, vec2* v_o, float ang)
{
	float cosa = cosf(-ang);
	float sina = sinf(-ang);
	v_o->x = v_i->x * cosa - v_i->y * sina;
	v_o->y = v_i->x * sina + v_i->y * cosa;
}

inline float calcDistance(vec2* v1, vec2* v2)
{
	return sqrt((v2->x - v1->x) * (v2->x - v1->x) + (v2->y - v1->y) * (v2->y - v1->y));
}

inline float calcDotProductVec2(vec2 v1, vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}