
#include "gamemath.h"

#include <nusys.h>

float clamp(float x, float min, float max) {
	return MIN(max, MAX(min, x));
}
