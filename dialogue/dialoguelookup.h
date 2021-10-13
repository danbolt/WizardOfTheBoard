
#ifndef _DIALOGUELOOKUP_H_
#define _DIALOGUELOOKUP_H_

#include "ultratypes.h"

struct dialogueMappingData { char* name; unsigned int offset; };

struct dialogueMappingData * getDialogueDataOffset (register const char *str, register size_t len);

#endif