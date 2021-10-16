#ifndef _MAPLOOKUP_H_
#define _MAPLOOKUP_H_

#include "ultratypes.h"

struct dialogueMappingData { char* name; unsigned int offset; };

struct dialogueMappingData * getMapDataOffset (register const char *str, register size_t len);

 #endif