
#include "pieces.h"

#define RM_ZB_TEX_EDGE(clk)          \
  Z_CMP | Z_UPD | IM_RD | CVG_DST_CLAMP |   \
  CVG_X_ALPHA | ALPHA_CVG_SEL | ZMODE_OPA | TEX_EDGE |  \
  GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define G_RM_ZB_TEX_EDGE RM_ZB_TEX_EDGE(1)
#define G_RM_ZB_TEX_EDGE2  RM_ZB_TEX_EDGE(2)

Vtx wall_verts[] = {
  { -100, -100,   0, 0, 64 << 5, 32 << 5, 4, 4, 4, 255 },
  { -100, -100, 200, 0, 64 << 5,  0 << 5, 87, 87, 87, 255 },
  { -100,  100,   0, 0, 96 << 5, 32 << 5, 0, 0, 0, 255 },
  { -100,  100, 200, 0, 96 << 5,  0 << 5, 87, 87, 87, 255 },
  {  100, -100,   0, 0, 96 << 5, 32 << 5, 0, 0, 0, 255 },
  {  100, -100, 200, 0, 96 << 5,  0 << 5, 86, 86, 86, 255 },
  {  100,  100,   0, 0, 64 << 5, 32 << 5, 0, 0, 0, 255 },
  {  100,  100, 200, 0, 64 << 5,  0 << 5, 82, 82, 82, 255 },
};

Gfx wall_commands[] = {
  gsDPPipeSync(),
  gsDPSetCombineMode(G_CC_MODULATEIA, G_CC_MODULATEIA),
  gsDPSetRenderMode(G_RM_ZB_TEX_EDGE, G_RM_ZB_TEX_EDGE2),
  gsSPVertex(wall_verts, 8, 0),
  gsSPClearGeometryMode(G_CULL_BACK),
  gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
  gsSP2Triangles(0, 3, 2, 0, 2, 7, 6, 0),
  gsSP2Triangles(6, 5, 4, 0, 4, 1, 0, 0),
  gsSP2Triangles(2, 4, 0, 0, 7, 1, 5, 0),
  gsSP2Triangles(0, 1, 3, 0, 2, 3, 7, 0),
  gsSP2Triangles(6, 7, 5, 0, 4, 5, 1, 0),
  gsSP2Triangles(2, 6, 4, 0, 7, 3, 1, 0),
  gsDPPipeSync(),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
  gsSPSetGeometryMode(G_CULL_BACK),
  gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF),
  gsSPEndDisplayList()
};


void wallLegalMove(u32 ourIndex, const u8* piecesActive, const Pos2* piecePositions, u8* legalSpots) {
  return;
}