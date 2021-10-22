
#include "monsters.h"

Vtx projectile_verts[] = {
  { 0, 0, 184, 0, 0, 0, 186, 180, 0, 255 },
  { 75, 0, 28, 0, 0, 0, 180, 180, 48, 255 },
  { 0, 78, 96, 0, 0, 0, 42, 186, 0, 255 },
  { 0, -77, 96, 0, 0, 0, 0, 175, 186, 255 },
  { -75, 0, 28, 0, 0, 0, 185, 180, 13, 255 },
};

Gfx projectile_commands[] = {
  gsSPVertex(projectile_verts, 5, 0),
  gsSP2Triangles(2, 0, 1, 0, 0, 3, 1, 0),
  gsSP2Triangles(2, 1, 4, 0, 1, 3, 4, 0),
  gsSP2Triangles(2, 4, 0, 0, 4, 3, 0, 0),
  gsSPEndDisplayList()
};

