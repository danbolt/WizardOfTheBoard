
#include "stagekeys.h"

#include "audio/bgm/sequence/tracknumbers.h"

LevelEntry levels[NUMBER_OF_LEVELS] = {
  { "tut1_move", TRACK_13_WARMING_UP, 0x0 },
  { "tut2_rooks", TRACK_13_WARMING_UP, 0x0 },
  { "tut3_bishops", TRACK_13_WARMING_UP, 0x0 },
  { "tut4_knights", TRACK_13_WARMING_UP, 0x0 },
  { "tut5_king", TRACK_13_WARMING_UP, 0x0 },

  { "1toad", TRACK_01_PUZZLE_TIME, 0x0 },
  { "2toad", TRACK_01_PUZZLE_TIME, 0x0 },
  { "jumper", TRACK_01_PUZZLE_TIME, 0x0 },
  { "snake", TRACK_01_PUZZLE_TIME, 0x0 },
  { "ogre", TRACK_01_PUZZLE_TIME, 0x0 },


  // promotion level here
  // also include flashback


  { "finale", TRACK_12_TOP_OF_THE_TOWER, 0x0 },
};