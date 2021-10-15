
#include "board.h"
#include "pieces.h"
#include "constants.h"

void bishopLegalMove(u32 ourIndex, const u8 *piecesActive, const Pos2 *piecePositions, u8 *legalSpots)
{
  const Pos2 *ourPosition = &(piecePositions[ourIndex]);

  // top right
  Pos2 checkTopRight = {ourPosition->x + 1, ourPosition->y + 1};
  while ((checkTopRight.x < BOARD_WIDTH) && (checkTopRight.y < BOARD_HEIGHT))
  {
    if (isSpaceOccupied(checkTopRight.x, checkTopRight.y) > -1)
    {
      break;
    }

    legalSpots[checkTopRight.x + (BOARD_WIDTH * checkTopRight.y)] = 1;
    checkTopRight.x++;
    checkTopRight.y++;
  }

  // top left
  Pos2 checkTopLeft = {ourPosition->x - 1, ourPosition->y + 1};
  while ((checkTopLeft.x >= 0) && (checkTopLeft.y < BOARD_HEIGHT))
  {
    if (isSpaceOccupied(checkTopLeft.x, checkTopLeft.y) > -1)
    {
      break;
    }

    legalSpots[checkTopLeft.x + (BOARD_WIDTH * checkTopLeft.y)] = 1;
    checkTopLeft.x--;
    checkTopLeft.y++;
  }

  // bottom right
  Pos2 checkBottomRight = {ourPosition->x + 1, ourPosition->y - 1};
  while ((checkBottomRight.x < BOARD_WIDTH) && (checkBottomRight.y >= 0))
  {
    if (isSpaceOccupied(checkBottomRight.x, checkBottomRight.y) > -1)
    {
      break;
    }

    legalSpots[checkBottomRight.x + (BOARD_WIDTH * checkBottomRight.y)] = 1;
    checkBottomRight.x++;
    checkBottomRight.y--;
  }

  // bottom left
  Pos2 checkBottomLeft = {ourPosition->x - 1, ourPosition->y - 1};
  while ((checkBottomLeft.x >= 0) && (checkBottomLeft.y >= 0))
  {
    if (isSpaceOccupied(checkBottomLeft.x, checkBottomLeft.y) > -1)
    {
      break;
    }

    legalSpots[checkBottomLeft.x + (BOARD_WIDTH * checkBottomLeft.y)] = 1;
    checkBottomLeft.x--;
    checkBottomLeft.y--;
  }
}