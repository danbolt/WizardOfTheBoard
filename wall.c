
#include "pieces.h"

#define RM_ZB_TEX_EDGE(clk)          \
  Z_CMP | Z_UPD | CVG_DST_CLAMP |   \
  CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE |  \
  GBL_c##clk(G_BL_CLR_IN, G_BL_0, G_BL_CLR_IN, G_BL_1)

#define G_RM_ZB_TEX_EDGE RM_ZB_TEX_EDGE(1)
#define G_RM_ZB_TEX_EDGE2  RM_ZB_TEX_EDGE(2)

Vtx wall_verts[] = {
  { -100, -100,   0, 0,  64 << 5,  32 << 5, 4, 4, 4, 255 },
  { -100, -100, 200, 0,  64 << 5,   0 << 5, 87, 87, 87, 255 },
  { -100,  100,   0, 0,  96 << 5,  32 << 5, 0, 0, 0, 255 },
  { -100,  100, 200, 0,  96 << 5,   0 << 5, 87, 87, 87, 255 },
  {  100, -100,   0, 0,  96 << 5,  32 << 5, 0, 0, 0, 255 },
  {  100, -100, 200, 0,  96 << 5,   0 << 5, 86, 86, 86, 255 },
  {  100,  100,   0, 0,  64 << 5,  32 << 5, 0, 0, 0, 255 },
  {  100,  100, 200, 0,  64 << 5,   0 << 5, 82, 82, 82, 255 },
  { -100,    0,   0, 0,  80 << 5,  32 << 5, 2, 2, 2, 255 },
  { -100, -100, 100, 0,  64 << 5,  16 << 5, 46, 46, 46, 255 },
  { -100,    0, 200, 0,  80 << 5,   0 << 5, 87, 87, 87, 255 },
  { -100,  100, 100, 0,  96 << 5,  16 << 5, 44, 44, 44, 255 },
  {    0,  100,   0, 0,  80 << 5,  32 << 5, 0, 0, 0, 255 },
  {    0,  100, 200, 0,  80 << 5,   0 << 5, 85, 85, 85, 255 },
  {  100,  100, 100, 0,  64 << 5,  16 << 5, 41, 41, 41, 255 },
  {  100,    0,   0, 0,  80 << 5,  32 << 5, 0, 0, 0, 255 },
  {  100,    0, 200, 0,  80 << 5,   0 << 5, 84, 84, 84, 255 },
  {  100, -100, 100, 0,  96 << 5,  16 << 5, 43, 43, 43, 255 },
  {    0, -100,   0, 0,  80 << 5,  32 << 5, 2, 2, 2, 255 },
  {    0, -100, 200, 0,  80 << 5,   0 << 5, 87, 87, 87, 255 },
  {  100,    0, 100, 0,  80 << 5,  16 << 5, 42, 42, 42, 255 },
  {    0,  100, 100, 0,  80 << 5,  16 << 5, 43, 43, 43, 255 },
  { -100,    0, 100, 0,  80 << 5,  16 << 5, 45, 45, 45, 255 },
  {    0, -100, 100, 0,  80 << 5,  16 << 5, 45, 45, 45, 255 },
};

Gfx wall_commands[] = {
  gsDPPipeSync(),
  gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
  gsDPSetRenderMode(G_RM_ZB_TEX_EDGE, G_RM_ZB_TEX_EDGE2),
  gsSPVertex(wall_verts, 24, 0),
  gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
  gsSP2Triangles(23, 4, 17, 0, 22, 0, 9, 0),
  gsSP2Triangles(21, 2, 11, 0, 16, 14, 7, 0),
  gsSP2Triangles(3, 22, 10, 0, 10, 9, 1, 0),
  gsSP2Triangles(17, 15, 20, 0, 13, 11, 3, 0),
  gsSP2Triangles(7, 21, 13, 0, 5, 20, 16, 0),
  gsSP2Triangles(14, 12, 21, 0, 20, 6, 14, 0),
  gsSP2Triangles(11, 8, 22, 0, 19, 17, 5, 0),
  gsSP2Triangles(1, 23, 19, 0, 9, 18, 23, 0),
  gsSP2Triangles(23, 18, 4, 0, 22, 8, 0, 0),
  gsSP2Triangles(21, 12, 2, 0, 16, 20, 14, 0),
  gsSP2Triangles(3, 11, 22, 0, 10, 22, 9, 0),
  gsSP2Triangles(17, 4, 15, 0, 13, 21, 11, 0),
  gsSP2Triangles(7, 14, 21, 0, 5, 17, 20, 0),
  gsSP2Triangles(14, 6, 12, 0, 20, 15, 6, 0),
  gsSP2Triangles(11, 2, 8, 0, 19, 23, 17, 0),
  gsSP2Triangles(1, 9, 23, 0, 9, 0, 18, 0),
  gsDPPipeSync(),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
  gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF),
  gsSPEndDisplayList()
};


void wallLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots) {
  return;
}