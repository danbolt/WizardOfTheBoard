
#include "gamemath.h"

#include <nusys.h>

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

float clamp(float x, float min, float max) {
	return MIN(max, MAX(min, x));
}
