
#ifndef _BACKGROUNDLOOKUP_H_
#define _BACKGROUNDLOOKUP_H_

#include "ultratypes.h"

struct backgroundMappingData { char* name; unsigned int offset; };

struct backgroundMappingData * getBackgroundTextureOffset (register const char *str, register size_t len);

#endif