/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --no-strlen --lookup-function-name=getBackgroundTextureOffset --struct-type --output-file=backgroundlookup.c texture-gperf-mapping  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "texture-gperf-mapping"
#include "backgroundlookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

#line 6 "texture-gperf-mapping"
struct backgroundMappingData;

#define TOTAL_KEYWORDS 17
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 10
#define MIN_HASH_VALUE 0
#define MAX_HASH_VALUE 60
/* maximum key range = 61, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 30,  3,  3,
      10, 20, 61,  3, 30, 61, 61, 61,  3, 30,
       5, 15, 30, 61, 15,  0, 20, 61, 61,  0,
      61, 61, 25, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61
    };
  return asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

struct backgroundMappingData *
getBackgroundTextureOffset (register const char *str, register size_t len)
{
  static struct backgroundMappingData wordlist[] =
    {
#line 20 "texture-gperf-mapping"
      {"stars",1843200},
      {""}, {""},
#line 23 "texture-gperf-mapping"
      {"wandering",2304000},
      {""},
#line 18 "texture-gperf-mapping"
      {"slain",1536000},
      {""}, {""}, {""}, {""},
#line 19 "texture-gperf-mapping"
      {"spotted",1689600},
      {""}, {""},
#line 9 "texture-gperf-mapping"
      {"cornered",153600},
      {""},
#line 11 "texture-gperf-mapping"
      {"dreamslain",460800},
      {""}, {""}, {""}, {""},
#line 17 "texture-gperf-mapping"
      {"revelation",1382400},
      {""}, {""},
#line 13 "texture-gperf-mapping"
      {"logotype",768000},
      {""},
#line 15 "texture-gperf-mapping"
      {"rescued",1075200},
      {""}, {""}, {""}, {""},
#line 10 "texture-gperf-mapping"
      {"dreamogre",307200},
      {""}, {""},
#line 8 "texture-gperf-mapping"
      {"bedroom",0},
      {""},
#line 14 "texture-gperf-mapping"
      {"outside",921600},
      {""}, {""}, {""}, {""},
#line 22 "texture-gperf-mapping"
      {"themouse",2150400},
      {""}, {""}, {""}, {""},
#line 16 "texture-gperf-mapping"
      {"restarea",1228800},
      {""}, {""}, {""}, {""},
#line 21 "texture-gperf-mapping"
      {"test_a",1996800},
      {""}, {""}, {""}, {""},
#line 24 "texture-gperf-mapping"
      {"zatts_room",2457600},
      {""}, {""}, {""}, {""},
#line 12 "texture-gperf-mapping"
      {"holdup",614400}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
