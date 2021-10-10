
#ifndef GAMEMATH_H
#define GAMEMATH_H

#include <nusys.h>

#define INV_PI (1.f / M_PI)

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	int x;
	int y;
} Pos2;

float lerp(float a, float b, float f);

float clamp(float x, float min, float max);

#endif