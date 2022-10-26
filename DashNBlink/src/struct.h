#pragma once

#include <utility>

struct vec2
{
	float x, y;
};

inline float calcDotProductVec2(vec2 v1, vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

inline float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}