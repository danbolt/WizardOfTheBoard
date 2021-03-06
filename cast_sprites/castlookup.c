/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --no-strlen --lookup-function-name=getCastTextureOffset --struct-type --output-file=castlookup.c texture-gperf-mapping  */
/* Computed positions: -k'1,6,$' */

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
#include "castlookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

#line 6 "texture-gperf-mapping"
struct castMappingData;

#define TOTAL_KEYWORDS 21
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 14
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
      61, 61, 61, 61, 61, 61, 61, 61, 30, 61,
       5,  8, 61,  0,  3, 61,  0, 61, 30, 61,
       3, 30, 10, 30, 25, 15,  0, 10, 61, 28,
      61, 10, 15, 61, 61, 61, 61, 61, 61, 61,
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
  register unsigned int hval = 0;

  switch (len)
    {
      default:
        hval += asso_values[(unsigned char)str[5]];
      /*FALLTHROUGH*/
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct castMappingData *
getCastTextureOffset (register const char *str, register size_t len)
{
  static struct castMappingData wordlist[] =
    {
#line 19 "texture-gperf-mapping"
      {"test",90112},
      {""}, {""},
#line 18 "texture-gperf-mapping"
      {"teach",81920},
      {""},
#line 22 "texture-gperf-mapping"
      {"toad",114688},
      {""}, {""},
#line 9 "texture-gperf-mapping"
      {"demon",8192},
      {""},
#line 12 "texture-gperf-mapping"
      {"protag",32768},
      {""}, {""},
#line 21 "texture-gperf-mapping"
      {"themouse_hah",106496},
      {""},
#line 13 "texture-gperf-mapping"
      {"protag_annoyed",40960},
      {""}, {""},
#line 20 "texture-gperf-mapping"
      {"themouse",98304},
      {""},
#line 14 "texture-gperf-mapping"
      {"protag_happy",49152},
      {""}, {""},
#line 17 "texture-gperf-mapping"
      {"snake",73728},
      {""},
#line 28 "texture-gperf-mapping"
      {"zatt_yelling",163840},
      {""}, {""},
#line 23 "texture-gperf-mapping"
      {"wait",122880},
      {""},
#line 27 "texture-gperf-mapping"
      {"zatt_smiling",155648},
      {""}, {""},
#line 16 "texture-gperf-mapping"
      {"queen",65536},
      {""},
#line 26 "texture-gperf-mapping"
      {"zatt_sad",147456},
      {""}, {""},
#line 11 "texture-gperf-mapping"
      {"ogre",24576},
      {""},
#line 15 "texture-gperf-mapping"
      {"protag_neutral",57344},
      {""}, {""}, {""}, {""},
#line 24 "texture-gperf-mapping"
      {"zatt_despair",131072},
      {""}, {""},
#line 25 "texture-gperf-mapping"
      {"zatt_neutral",139264},
      {""},
#line 10 "texture-gperf-mapping"
      {"jumper",16384},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 8 "texture-gperf-mapping"
      {"b",0}
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
