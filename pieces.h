
#ifndef PIECES_H
#define PIECES_H

#include <nusys.h>

#include "gamemath.h"

extern Vtx pawn_verts[];
extern Gfx pawn_commands[];

extern Vtx rook_verts[];
extern Gfx rook_commands[];

typedef enum {
	PAWN = 0,
	ROOK = 1,
	KNIGHT = 2,
	BISHOP =3,
	QUEEN = 4,
	KING = 5,
} PieceType;

typedef int (*LegalMoveCheck)(u32 ourIndex, const Pos2* desiredSpot, const u8* piecesActive, const Pos2* piecePositions);

typedef struct {
	PieceType type;
	LegalMoveCheck legalCheck;

	Gfx* renderCommands;
} PieceInfo;

#endif