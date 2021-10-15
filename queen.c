#include "board.h"
#include "pieces.h"
#include "constants.h"

void queenLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots) {
    rookLegalMove(ourIndex, piecesActive, piecePositions, legalSpots);
    bishopLegalMove(ourIndex, piecesActive, piecePositions, legalSpots);
}
