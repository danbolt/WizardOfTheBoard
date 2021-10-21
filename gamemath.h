
#ifndef GAMEMATH_H
#define GAMEMATH_H

#include <nusys.h>

#define INV_PI (1.f / M_PI)

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	int x;
	int y;
} Pos2;

float lerp(float a, float b, float f);

// Adapted from: 
// https://stackoverflow.com/questions/20825951/fastest-way-to-interpolate-between-radians
float lerpAngle(float u, float v, float p);

float clamp(float x, float min, float max);

float distanceSq(const Vec2* a, const Vec2* b);

float normalize(Vec2* a);

#endif