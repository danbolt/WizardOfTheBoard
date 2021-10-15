#include "board.h"
#include "pieces.h"
#include "constants.h"

static const Pos2 kingOffsets[] = {
    {  1, 0 },
    { -1, 0 },
    {  0, 1 },
    {  0,-1 },

    {  1, 1 },
    { -1, 1 },
    {  1,-1 },
    { -1,-1 },
};

void kingLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots) {
    const Pos2* ourPosition = &(piecePositions[ourIndex]);

    for (int i = 0; i < 8; i++) {
        int spotX = ourPosition->x + kingOffsets[i].x;
        if (spotX < 0 || spotX >= BOARD_WIDTH) {
            continue;
        }
        int spotY = ourPosition->y + kingOffsets[i].y;
        if (spotY < 0 || spotY >= BOARD_HEIGHT) {
            continue;
        }

        if (isSpaceOccupied(spotX, spotY) > -1) {
            continue;
        }

        legalSpots[spotX + (BOARD_WIDTH * spotY)] = 1;
    }
}
