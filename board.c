

#include "board.h"
#include "pieces.h"

u8 piecesActive[MAX_NUMBER_OF_INGAME_PIECES];
Pos2 piecePositions[MAX_NUMBER_OF_INGAME_PIECES];
Vec2 pieceViewPos[MAX_NUMBER_OF_INGAME_PIECES];
u8 pieceIsLerping[MAX_NUMBER_OF_INGAME_PIECES];
Vec2 oldPiecePos[MAX_NUMBER_OF_INGAME_PIECES];
float pieceLerpValue[MAX_NUMBER_OF_INGAME_PIECES];
PieceInfo pieceData[MAX_NUMBER_OF_INGAME_PIECES];

void initPieceStates() {
  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    piecesActive[i] = 0;
    pieceIsLerping[i] = 0;
    piecePositions[i] = (Pos2){ 0, 0 };
    pieceViewPos[i] = (Vec2){ 0.f, 0.f };
    pieceLerpValue[i] = 0.f;
    pieceData[i].type = ROOK;
    pieceData[i].renderCommands = rook_commands;
    pieceData[i].legalCheck = rookLegalMove;
    pieceData[i].displayName = "";
    pieceData[i].selectable = 1;
  }
}

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


int isSpaceOccupiedButIgnoreMovingPieces(int x, int y)  {

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

    if (pieceIsLerping[i]) {
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