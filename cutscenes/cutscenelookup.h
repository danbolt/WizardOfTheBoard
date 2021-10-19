
#ifndef _CUTSCENELOOKUP_H_
#define _CUTSCENELOOKUP_H_

#include "ultratypes.h"

struct cutsceneMappingData { char* name; unsigned int offset; };

struct cutsceneMappingData * getCutsceneOffset (register const char *str, register size_t len);

#endif