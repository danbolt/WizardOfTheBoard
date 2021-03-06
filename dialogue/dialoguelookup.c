/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --no-strlen --lookup-function-name=getDialogueDataOffset --struct-type --output-file=dialoguelookup.c map-gperf-mapping  */
/* Computed positions: -k'5,$' */

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
#include "dialoguelookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

#line 6 "map-gperf-mapping"
struct dialogueMappingData;

#define TOTAL_KEYWORDS 24
#define MIN_WORD_LENGTH 4
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
       5,  8, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 20, 61, 61, 61, 18,
      23, 30, 61,  8, 25, 61, 61,  3,  8, 15,
       0, 30,  3, 61,  3, 10,  5, 15,  0, 30,
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
      61, 61, 61, 61, 61, 61, 61, 61, 61, 61,
      61, 61, 61, 61, 61, 61
    };
  register unsigned int hval = 0;

  switch (len)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct dialogueMappingData *
getDialogueDataOffset (register const char *str, register size_t len)
{
  static struct dialogueMappingData wordlist[] =
    {
#line 10 "map-gperf-mapping"
      {"begin",8932},
      {""}, {""},
#line 18 "map-gperf-mapping"
      {"itsover",35420},
      {""},
#line 27 "map-gperf-mapping"
      {"rest",62216},
#line 12 "map-gperf-mapping"
      {"check",15708},
      {""},
#line 17 "map-gperf-mapping"
      {"individual",34804},
      {""},
#line 28 "map-gperf-mapping"
      {"rest2",68376},
      {""}, {""},
#line 8 "map-gperf-mapping"
      {"afterwards",0},
      {""},
#line 30 "map-gperf-mapping"
      {"thequeen",78848},
#line 29 "map-gperf-mapping"
      {"rest3",73304},
      {""}, {""}, {""},
#line 26 "map-gperf-mapping"
      {"rescue2",49280},
      {""}, {""},
#line 20 "map-gperf-mapping"
      {"meet_jumper",36344},
      {""},
#line 31 "map-gperf-mapping"
      {"uhhh",83160},
      {""}, {""},
#line 16 "map-gperf-mapping"
      {"flashback_war",32032},
      {""},
#line 13 "map-gperf-mapping"
      {"dream",24024},
      {""}, {""},
#line 19 "map-gperf-mapping"
      {"longpiece",35728},
      {""},
#line 15 "map-gperf-mapping"
      {"flashback_loss",29568},
      {""}, {""},
#line 23 "map-gperf-mapping"
      {"meetog",42196},
      {""},
#line 14 "map-gperf-mapping"
      {"flashback_form",27720},
      {""}, {""},
#line 22 "map-gperf-mapping"
      {"meet_toad",39424},
      {""},
#line 25 "map-gperf-mapping"
      {"rescue",44044},
      {""}, {""},
#line 11 "map-gperf-mapping"
      {"chance",9856},
      {""},
#line 21 "map-gperf-mapping"
      {"meet_snake",36960},
      {""}, {""}, {""}, {""},
#line 9 "map-gperf-mapping"
      {"approach",6776},
      {""}, {""}, {""}, {""},
#line 24 "map-gperf-mapping"
      {"powpowpow",43428}
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
