
#ifndef MAPDATA_H
#define MAPDATA_H

#include <nusys.h>

#include "constants.h"

typedef struct {
  u8 activePieces[MAX_NUMBER_OF_INGAME_PIECES];
  u8 pieceX[MAX_NUMBER_OF_INGAME_PIECES];
  u8 pieceY[MAX_NUMBER_OF_INGAME_PIECES];
  u8 pieceType[MAX_NUMBER_OF_INGAME_PIECES];

  u8 activeMonsters[MAX_NUMBER_OF_INGAME_MONSTERS];
  u8 monsterType[MAX_NUMBER_OF_INGAME_MONSTERS];
  u8 monsterX[MAX_NUMBER_OF_INGAME_MONSTERS];
  u8 monsterY[MAX_NUMBER_OF_INGAME_MONSTERS];

  u8 puzzleSpotX[MAX_NUMBER_OF_PUZZLE_SPACES];
  u8 puzzleSpotY[MAX_NUMBER_OF_PUZZLE_SPACES];

  u8 numberOfPuzzleSpots;
  u8 playerX;
  u8 playerY;
  u8 playerRotation;

  u8 startLevelDialogue[16];

  u8 flagA;
  u8 flagB;
  u8 flagC;
  u8 bgm;

} MapData;

#endif