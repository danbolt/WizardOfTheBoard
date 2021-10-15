
#ifndef _NUSTDFUNCS_H_
#define _NUSTDFUNCS_H_

#include "ultratypes.h"

#define TOL ((float)1.0E-7)    /* Fix precision to 10^-7 because of the float type  */
#define M_PI_2    1.57079632679489661923
#define M_PI_4    0.78539816339744830962
#define M_RTOD    (180.0/3.14159265358979323846)

float fabsf(float x);

int strcmp(const char *str1, const char *str2);

size_t  _nstrlen(const char *string);

char *strncpy(char *str1, const char *str2, size_t  n);

float nu_atan2(float y, float x);

#endif