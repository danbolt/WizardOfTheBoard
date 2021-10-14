
#ifndef _CASTLOOKUP_H_
#define _CASTLOOKUP_H_

#include "ultratypes.h"

struct castMappingData { char* name; unsigned int offset; };

struct castMappingData * getCastTextureOffset (register const char *str, register size_t len);

#endif