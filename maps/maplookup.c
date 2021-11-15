/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --no-strlen --lookup-function-name=getMapDataOffset --struct-type --output-file=maplookup.c map-gperf-mapping  */
/* Computed positions: -k'1,4,7' */

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

#line 1 "map-gperf-mapping"
#include "maplookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

#line 6 "map-gperf-mapping"
struct dialogueMappingData;

#define TOTAL_KEYWORDS 19
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 0
#define MAX_HASH_VALUE 55
/* maximum key range = 56, duplicates = 0 */

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
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 10,
       0,  3, 23, 25, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 20,  5,  8, 56,
      56, 20, 15, 20,  5, 56,  3,  3, 56, 56,
      56, 10,  0, 56, 56, 15,  0,  5, 25, 15,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
      56, 56, 56, 56, 56, 56, 56
    };
  register unsigned int hval = 0;

  switch (len)
    {
      default:
        hval += asso_values[(unsigned char)str[6]+1];
      /*FALLTHROUGH*/
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct dialogueMappingData *
getMapDataOffset (register const char *str, register size_t len)
{
  static struct dialogueMappingData wordlist[] =
    {
#line 22 "map-gperf-mapping"
      {"tut2_rooks",2352},
      {""}, {""},
#line 12 "map-gperf-mapping"
      {"jumper",672},
      {""},
#line 9 "map-gperf-mapping"
      {"2toad",168},
#line 23 "map-gperf-mapping"
      {"tut3_bishops",2520},
      {""},
#line 20 "map-gperf-mapping"
      {"test_map",2016},
      {""},
#line 21 "map-gperf-mapping"
      {"tut1_move",2184},
      {""}, {""},
#line 10 "map-gperf-mapping"
      {"bishops",336},
      {""},
#line 8 "map-gperf-mapping"
      {"1toad",0},
      {""}, {""},
#line 18 "map-gperf-mapping"
      {"snake",1680},
      {""},
#line 11 "map-gperf-mapping"
      {"finale",504},
      {""}, {""},
#line 13 "map-gperf-mapping"
      {"jumper_test",840},
      {""},
#line 26 "map-gperf-mapping"
      {"twoogres",3024},
      {""}, {""},
#line 25 "map-gperf-mapping"
      {"tut5_king",2856},
      {""},
#line 14 "map-gperf-mapping"
      {"ogre",1008},
      {""}, {""},
#line 24 "map-gperf-mapping"
      {"tut4_knights",2688},
      {""},
#line 16 "map-gperf-mapping"
      {"other_test_map",1344},
      {""}, {""}, {""}, {""},
#line 15 "map-gperf-mapping"
      {"oneweirdtrick",1176},
      {""}, {""}, {""}, {""},
#line 19 "map-gperf-mapping"
      {"solveit",1848},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 17 "map-gperf-mapping"
      {"singlefile",1512}
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
