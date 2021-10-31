
#include "gamemath.h"

#include <nusys.h>

// Quake III Fast-inverse square root
// Copied from Wikipedia: https://en.wikipedia.org/wiki/Fast_inverse_square_root#Overview_of_the_code
float Q_rsqrt( float number ) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   

	return y;
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

// Very crass wrapping function
float wrapMP(float f) {
	while (f > M_PI) {
		f -= (M_PI * 2.f);
	} 

	while (f < -M_PI) {
		f += (M_PI * 2.f);
	}

	return f;
}

float lerpAngle(float u, float v, float p) {
    return u + p*wrapMP(v - u);
}

float clamp(float x, float min, float max) {
	return MIN(max, MAX(min, x));
}

float distanceSq(const Vec2* a, const Vec2* b) {
	const float deltaX = (a->x - b->x);
	const float deltaY = (a->y - b->y);

	return (deltaX * deltaX) + (deltaY * deltaY);
}

float lengthSq(const Vec2* a) {
	const Vec2 ZERO = { 0.f, 0.f };
	return distanceSq(a, &ZERO);
}

void normalize(Vec2* a) {
	const float lengthSquared = lengthSq(a);
	const float invLengthA = Q_rsqrt(lengthSquared);
	a->x *= invLengthA;
	a->y *= invLengthA;
}

float cubic(float t) {
	return t * t * t;
}