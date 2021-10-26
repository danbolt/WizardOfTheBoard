
#ifndef _BIPMAPPING_H_
#define _BIPMAPPING_H_

#include "ultratypes.h"

struct bipMapping { char* name; u32 sfxKey; };

struct bipMapping * getBipMapping (register const char *str, register size_t len);

#endif