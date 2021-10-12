
#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "gamemath.h"
#include "pieces.h"

extern u8 piecesActive[MAX_NUMBER_OF_INGAME_PIECES];
extern Pos2 piecePositions[MAX_NUMBER_OF_INGAME_PIECES];
extern Vec2 pieceViewPos[MAX_NUMBER_OF_INGAME_PIECES];
extern u8 pieceIsLerping[MAX_NUMBER_OF_INGAME_PIECES];
extern Vec2 oldPiecePos[MAX_NUMBER_OF_INGAME_PIECES];
extern float pieceLerpValue[MAX_NUMBER_OF_INGAME_PIECES];
extern PieceInfo pieceData[MAX_NUMBER_OF_INGAME_PIECES];

void initPieceStates();

int tileIsLight(int x, int y);
int tileIsDark(int x, int y);

// returns -1 if empty, otherwise the index of the occupying piece
int isSpaceOccupied(int x, int y);
int isSpaceOccupiedButIgnoreMovingPieces(int x, int y);

void boardPosToLetter(const Pos2* spot, char* x, char* y);

#endif