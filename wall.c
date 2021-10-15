
#include "pieces.h"

Vtx wall_verts[] = {
  { -100, -100, 0, 0, 0, 0, 4, 4, 4, 255 },
  { -100, -100, 400, 0, 0, 0, 87, 87, 87, 255 },
  { -100, 100, 0, 0, 0, 0, 0, 0, 0, 255 },
  { -100, 100, 400, 0, 0, 0, 87, 87, 87, 255 },
  { 100, -100, 0, 0, 0, 0, 0, 0, 0, 255 },
  { 100, -100, 400, 0, 0, 0, 86, 86, 86, 255 },
  { 100, 100, 0, 0, 0, 0, 0, 0, 0, 255 },
  { 100, 100, 400, 0, 0, 0, 82, 82, 82, 255 },
};

Gfx wall_commands[] = {
  gsSPVertex(wall_verts, 8, 0),
  gsSP2Triangles(0, 3, 2, 0, 2, 7, 6, 0),
  gsSP2Triangles(6, 5, 4, 0, 4, 1, 0, 0),
  gsSP2Triangles(2, 4, 0, 0, 7, 1, 5, 0),
  gsSP2Triangles(0, 1, 3, 0, 2, 3, 7, 0),
  gsSP2Triangles(6, 7, 5, 0, 4, 5, 1, 0),
  gsSP2Triangles(2, 6, 4, 0, 7, 3, 1, 0),
  gsSPEndDisplayList()
};


void wallLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots) {
  return;
}