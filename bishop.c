
#include "board.h"
#include "pieces.h"
#include "constants.h"

Vtx bishop_verts[] = {
  { 100, 0, 0, 0, 0, 0, 43, 43, 43, 255 },
  { 100, 0, 35, 0, 0, 0, 189, 189, 189, 255 },
  { 57, 0, 59, 0, 0, 0, 115, 115, 115, 255 },
  { 23, 0, 245, 0, 0, 0, 110, 110, 110, 255 },
  { 38, 35, 262, 0, 0, 0, 189, 189, 189, 255 },
  { 44, 43, 319, 0, 0, 0, 191, 191, 191, 255 },
  { 37, 0, 79, 0, 0, 0, 199, 199, 199, 255 },
  { 55, 0, 262, 0, 0, 0, 189, 189, 189, 255 },
  { 38, -35, 262, 0, 0, 0, 189, 189, 189, 255 },
  { 65, 1, 319, 0, 0, 0, 240, 240, 240, 255 },
  { 44, -39, 319, 0, 0, 0, 240, 240, 240, 255 },
  { 15, 5, 473, 0, 0, 0, 255, 255, 255, 255 },
  { 45, -4, 354, 0, 0, 0, 60, 60, 60, 255 },
  { 39, -26, 353, 0, 0, 0, 135, 135, 135, 255 },
  { -100, 0, 0, 0, 0, 0, 43, 43, 43, 255 },
  { -100, 0, 35, 0, 0, 0, 189, 189, 189, 255 },
  { -57, 0, 59, 0, 0, 0, 115, 115, 115, 255 },
  { -23, 0, 245, 0, 0, 0, 110, 110, 110, 255 },
  { -38, 35, 262, 0, 0, 0, 189, 189, 189, 255 },
  { -44, 43, 319, 0, 0, 0, 191, 191, 191, 255 },
  { -37, 0, 79, 0, 0, 0, 199, 199, 199, 255 },
  { 0, 100, 0, 0, 0, 0, 40, 40, 40, 255 },
  { 0, 100, 35, 0, 0, 0, 192, 192, 192, 255 },
  { 0, 47, 59, 0, 0, 0, 115, 115, 115, 255 },
  { 0, 21, 245, 0, 0, 0, 121, 121, 121, 255 },
  { 0, 56, 262, 0, 0, 0, 189, 189, 189, 255 },
  { 0, 78, 319, 0, 0, 0, 240, 240, 240, 255 },
  { 0, 28, 79, 0, 0, 0, 197, 197, 197, 255 },
  { 0, 5, 454, 0, 0, 0, 136, 136, 136, 255 },
  { -55, 0, 262, 0, 0, 0, 189, 189, 189, 255 },
  { -38, -35, 262, 0, 0, 0, 189, 189, 189, 255 },
  { -65, 1, 319, 0, 0, 0, 240, 240, 240, 255 },
  { -44, -39, 319, 0, 0, 0, 240, 240, 240, 255 },
  { 0, 0, 0, 0, 0, 0, 50, 50, 50, 255 },
  { 0, -100, 0, 0, 0, 0, 40, 40, 40, 255 },
  { 0, -100, 35, 0, 0, 0, 192, 192, 192, 255 },
  { 0, -47, 59, 0, 0, 0, 115, 115, 115, 255 },
  { 0, -20, 245, 0, 0, 0, 121, 121, 121, 255 },
  { 0, -55, 262, 0, 0, 0, 189, 189, 189, 255 },
  { 0, -72, 319, 0, 0, 0, 237, 237, 237, 255 },
  { 0, -28, 79, 0, 0, 0, 197, 197, 197, 255 },
  { 0, -35, 409, 0, 0, 0, 177, 177, 177, 255 },
  { 0, 5, 492, 0, 0, 0, 255, 255, 255, 255 },
  { 0, -12, 473, 0, 0, 0, 255, 255, 255, 255 },
  { -15, 5, 473, 0, 0, 0, 255, 255, 255, 255 },
  { 0, 24, 473, 0, 0, 0, 255, 255, 255, 255 },
  { -45, -4, 354, 0, 0, 0, 60, 60, 60, 255 },
  { -39, -26, 353, 0, 0, 0, 135, 135, 135, 255 },
  { 0, -7, 355, 0, 0, 0, 45, 45, 45, 255 },
};

Gfx bishop_commands[] = {
  gsSPVertex(bishop_verts, 49, 0),
  gsSP2Triangles(33, 21, 0, 0, 6, 36, 2, 0),
  gsSP2Triangles(21, 1, 0, 0, 3, 40, 6, 0),
  gsSP2Triangles(8, 37, 3, 0, 7, 8, 3, 0),
  gsSP2Triangles(10, 38, 8, 0, 9, 8, 7, 0),
  gsSP2Triangles(24, 4, 3, 0, 10, 12, 13, 0),
  gsSP2Triangles(33, 0, 34, 0, 22, 2, 1, 0),
  gsSP2Triangles(5, 12, 9, 0, 1, 34, 0, 0),
  gsSP2Triangles(25, 5, 4, 0, 3, 4, 7, 0),
  gsSP2Triangles(4, 9, 7, 0, 5, 26, 28, 0),
  gsSP2Triangles(27, 3, 6, 0, 2, 35, 1, 0),
  gsSP2Triangles(23, 6, 2, 0, 39, 13, 41, 0),
  gsSP2Triangles(28, 45, 11, 0, 28, 11, 43, 0),
  gsSP2Triangles(42, 43, 11, 0, 12, 28, 48, 0),
  gsSP2Triangles(11, 45, 42, 0, 13, 12, 48, 0),
  gsSP2Triangles(28, 12, 5, 0, 41, 13, 48, 0),
  gsSP2Triangles(33, 14, 21, 0, 20, 36, 40, 0),
  gsSP2Triangles(21, 15, 22, 0, 17, 40, 37, 0),
  gsSP2Triangles(30, 37, 38, 0, 29, 17, 30, 0),
  gsSP2Triangles(32, 38, 39, 0, 31, 30, 32, 0),
  gsSP2Triangles(24, 18, 25, 0, 32, 46, 31, 0),
  gsSP2Triangles(33, 34, 14, 0, 22, 16, 23, 0),
  gsSP2Triangles(19, 31, 46, 0, 15, 34, 35, 0),
  gsSP2Triangles(25, 19, 26, 0, 17, 29, 18, 0),
  gsSP2Triangles(18, 31, 19, 0, 19, 28, 26, 0),
  gsSP2Triangles(27, 17, 24, 0, 16, 35, 36, 0),
  gsSP2Triangles(23, 20, 27, 0, 39, 47, 32, 0),
  gsSP2Triangles(28, 44, 45, 0, 28, 43, 44, 0),
  gsSP2Triangles(42, 44, 43, 0, 46, 48, 28, 0),
  gsSP2Triangles(44, 42, 45, 0, 47, 48, 46, 0),
  gsSP2Triangles(28, 19, 46, 0, 41, 48, 47, 0),
  gsSP2Triangles(6, 40, 36, 0, 21, 22, 1, 0),
  gsSP2Triangles(3, 37, 40, 0, 8, 38, 37, 0),
  gsSP2Triangles(10, 39, 38, 0, 9, 10, 8, 0),
  gsSP2Triangles(24, 25, 4, 0, 10, 9, 12, 0),
  gsSP2Triangles(22, 23, 2, 0, 1, 35, 34, 0),
  gsSP2Triangles(25, 26, 5, 0, 4, 5, 9, 0),
  gsSP2Triangles(27, 24, 3, 0, 2, 36, 35, 0),
  gsSP2Triangles(23, 27, 6, 0, 39, 10, 13, 0),
  gsSP2Triangles(20, 16, 36, 0, 21, 14, 15, 0),
  gsSP2Triangles(17, 20, 40, 0, 30, 17, 37, 0),
  gsSP2Triangles(32, 30, 38, 0, 31, 29, 30, 0),
  gsSP2Triangles(24, 17, 18, 0, 32, 47, 46, 0),
  gsSP2Triangles(22, 15, 16, 0, 15, 14, 34, 0),
  gsSP2Triangles(25, 18, 19, 0, 18, 29, 31, 0),
  gsSP2Triangles(27, 20, 17, 0, 16, 15, 35, 0),
  gsSP2Triangles(23, 16, 20, 0, 39, 41, 47, 0),
  gsSPEndDisplayList(),
};


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