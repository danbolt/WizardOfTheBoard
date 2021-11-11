/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --no-strlen --output-file=bipmapping.c --lookup-function-name=getBipMapping --struct-type gperf-bip-mapping  */
/* Computed positions: -k'8,$' */

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

#line 1 "gperf-bip-mapping"
#include "ultratypes.h"
#include "audio/sfx/sfx.h"
#include "nustdfuncs.h"
#include "bipmapping.h"
#line 6 "gperf-bip-mapping"
struct bipMapping;

#define TOTAL_KEYWORDS 12
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 0
#define MAX_HASH_VALUE 30
/* maximum key range = 31, duplicates = 0 */

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
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31,  4,  3, 31,
      15,  5, 31, 10, 15, 31, 31, 31,  4, 31,
       0, 31, 31, 31, 31, 31, 31,  5, 31, 31,
      31, 10, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
      31, 31, 31, 31, 31, 31
    };
  register unsigned int hval = 0;

  switch (len)
    {
      default:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct bipMapping *
getBipMapping (register const char *str, register size_t len)
{
  static struct bipMapping wordlist[] =
    {
#line 16 "gperf-bip-mapping"
      {"queen",SFX_35_QUEEN_BIP},
      {""}, {""},
#line 15 "gperf-bip-mapping"
      {"b",SFX_32_B_BIP},
#line 14 "gperf-bip-mapping"
      {"protag_neutral",SFX_02_NOBODY_BIP},
#line 11 "gperf-bip-mapping"
      {"snake",SFX_18_OGRE_BIP},
      {""}, {""}, {""},
#line 8 "gperf-bip-mapping"
      {"zatt_neutral",SFX_13_ZATT_BIP},
#line 18 "gperf-bip-mapping"
      {"themouse",SFX_34_MOUSE_BIP},
      {""}, {""}, {""},
#line 10 "gperf-bip-mapping"
      {"zatt_yelling",SFX_14_ZATT_SCREAMING_BIP},
#line 17 "gperf-bip-mapping"
      {"teach",SFX_16_ELDER_BIP},
      {""}, {""}, {""},
#line 12 "gperf-bip-mapping"
      {"protag_annoyed",SFX_02_NOBODY_BIP},
#line 19 "gperf-bip-mapping"
      {"themouse_hah",SFX_34_MOUSE_BIP},
      {""}, {""}, {""}, {""},
#line 13 "gperf-bip-mapping"
      {"protag_happy",SFX_02_NOBODY_BIP},
      {""}, {""}, {""}, {""},
#line 9 "gperf-bip-mapping"
      {"zatt_sad",SFX_13_ZATT_BIP}
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
