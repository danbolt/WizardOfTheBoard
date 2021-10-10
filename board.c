

#include "board.h"


u8 piecesActive[MAX_NUMBER_OF_INGAME_PIECES];
Pos2 piecePositions[MAX_NUMBER_OF_INGAME_PIECES];
PieceInfo pieceData[MAX_NUMBER_OF_INGAME_PIECES];

// copied from:
// https://gamedev.stackexchange.com/questions/44979/elegant-solution-for-coloring-chess-tiles
int tileIsLight(int x, int y) {
  return (x % 2) == (y % 2);
}

int tileIsDark(int x, int y) {
  return (x % 2) == (y % 2);
}

int isSpaceOccupied(int x, int y) {

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    if (piecePositions[i].x != x) {
      continue;
    }

    if (piecePositions[i].y != y) {
      continue;
    }

    // If we've made it here, we've found an occupying piece
    return i;
  }

  return -1;
}

#define ASCII_START_CAPTIALS 65
#define ASCCI_START_NUMBERS 48

void boardPosToLetter(const Pos2* spot, char* x, char* y) {
  *x = (char)(spot->x + ASCII_START_CAPTIALS);
  *y = (char)(spot->y + 1 + ASCCI_START_NUMBERS);
}