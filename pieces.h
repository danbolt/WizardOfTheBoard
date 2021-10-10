
#ifndef PIECES_H
#define PIECES_H

#include <nusys.h>

#include "gamemath.h"

extern Vtx pawn_verts[];
extern Gfx pawn_commands[];
void pawnLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots);

extern Vtx rook_verts[];
extern Gfx rook_commands[];
void rookLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots);

typedef enum {
	PAWN = 0,
	ROOK = 1,
	KNIGHT = 2,
	BISHOP =3,
	QUEEN = 4,
	KING = 5,
} PieceType;

typedef void (*LegalMoveCheck)(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots);

typedef struct {
	PieceType type;
	LegalMoveCheck legalCheck;

	Gfx* renderCommands;
} PieceInfo;

#endif