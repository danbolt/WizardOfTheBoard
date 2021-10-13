
#include "nustdfuncs.h"

// Copied from the nustd library
int strcmp(const char *str1, const char *str2)
{
    unsigned char   c1 = 1,c2;
    while(c1)   {
        c1=*str1++;
        c2=*str2++;
        if (c1 != c2)   return c1 - c2;
    }
    return 0;
}

size_t  _nstrlen(const char *string)
{
    size_t  len = 0;
    while(*string++)    len++;
    return  len;
}

char *strncpy(char *str1, const char *str2, size_t  n)
{
    char *p = str1;
    while((*str2) && n--)   *p++ = *str2++;
    *p = '\0';
    return  str1;
}

