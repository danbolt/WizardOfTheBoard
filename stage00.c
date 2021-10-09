#include <assert.h>
#include <nusys.h>

#include "main.h"
#include "gamemath.h"
#include "graphic.h"
#include "tracknumbers.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

#define PLAYER_HEIGHT_ABOVE_GROUND 0.34f
#define PLAYER_WALK_SPEED 0.05f

static Vec2 playerPosition;
static float playerOrientation;
static float cosCameraRot;
static float sinCameraRot;

#define VERTS_PER_FLOOR_TILE 4
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define NUMBER_OF_BOARD_CELLS (BOARD_WIDTH * BOARD_HEIGHT)
#define NUMBER_OF_FLOOR_VERTS (NUMBER_OF_BOARD_CELLS * VERTS_PER_FLOOR_TILE)
static Vtx floorVerts[NUMBER_OF_FLOOR_VERTS];

#define VERT_BUFFER_SIZE 64

#define COMMANDS_END_DL_SIZE 1
static Gfx floorDL[(NUMBER_OF_BOARD_CELLS * 2) + COMMANDS_END_DL_SIZE];

static int foo;

// TODO: let us customize/randomize the textures for this on init time
void generateFloorTiles() {
  Gfx* commands = floorDL;
  Vtx* verts = floorVerts;
  Vtx* lastLoad = verts;

  for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
    const int x = (i % BOARD_WIDTH);
    const int y = (i / BOARD_WIDTH);

    *(verts++) = (Vtx){ x + 0, y + 0,  0, 0, 0, 0, 0xff, 0x00, 0x00, 0xff };
    *(verts++) = (Vtx){ x + 1, y + 0,  0, 0, 0, 0, 0x00, 0xff, 0x00, 0xff };
    *(verts++) = (Vtx){ x + 1, y + 1,  0, 0, 0, 0, 0x00, 0x00, 0xff, 0xff };
    *(verts++) = (Vtx){ x + 0, y + 1,  0, 0, 0, 0, 0x00, 0xff, 0xff, 0xff };

    if ((verts - lastLoad) >= VERT_BUFFER_SIZE) {
      gSPVertex(commands++, &(lastLoad[0]), VERT_BUFFER_SIZE, 0);
      for (int j = 0; j < VERT_BUFFER_SIZE; j += 4) {
        gSP2Triangles(commands++, j + 0, j + 1, j + 2, 0, j + 0, j + 2, j + 3, 0);
      }

      lastLoad = verts;
    }
  }

  gSPEndDisplayList(commands++);
}

/* The initialization of stage 0 */
void initStage00(void)
{
  generateFloorTiles();

  playerPosition = (Vec2){ 0.f, 0.f };
  playerOrientation = 0.f;
  cosCameraRot = 1.f;
  sinCameraRot = 0.f;
}



/* Make the display list and activate the task */
void makeDL00(void)
{
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  // This is used for `gSPPerspNormalize` 
  u16 perspectiveNorm = 0;

  guOrtho(&dynamicp->ortho, -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F, -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F, 1.0F, 10.0F, 1.0F);
  guPerspective(&dynamicp->projection, &perspectiveNorm, ingameFOV, ((float)SCREEN_WD)/((float)SCREEN_HT), 0.3f, 100.f, 1.f);
  guLookAt(&dynamicp->camera, playerPosition.x, playerPosition.y, PLAYER_HEIGHT_ABOVE_GROUND, playerPosition.x - sinCameraRot, playerPosition.y + cosCameraRot, PLAYER_HEIGHT_ABOVE_GROUND, 0.f, 0.f, 1.f);
  guMtxIdent(&dynamicp->modelling);

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );
  //gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modelling)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);
  gSPClipRatio(glistp++, FRUSTRATIO_6);

  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(floorDL));

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
      nuDebConTextPos(0,4,4);
      sprintf(conbuf,"x: %2.2f, y:%2.2f", playerPosition.x, playerPosition.y);
      nuDebConCPuts(0, conbuf);
    }
  else
    {
      nuDebConTextPos(0,9,24);
      nuDebConCPuts(0, "Controller1 not connect");
    }
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}

// TODO: Make this delta-dependent
void updatePlayerInput() {
  Vec2 step = { 0.f, 0.f };

  // Update rotation
  if(contdata[0].button & L_TRIG) {
    playerOrientation += 0.05f;

    if (playerOrientation > M_PI) {
      playerOrientation = -M_PI;
    }
  } else if(contdata[0].button & R_TRIG) {
    playerOrientation -= 0.05f;

    if (playerOrientation < -M_PI) {
      playerOrientation = M_PI;
    }
  }
  cosCameraRot = cosf(playerOrientation);
  sinCameraRot = sinf(playerOrientation);

  // Update position
  if(contdata[0].button & U_JPAD) {
    step.y = 1.f;
  } else if(contdata[0].button & D_JPAD) {
    step.y = -1.f;
  }

  if(contdata[0].button & R_JPAD) {
    step.x = 1.f;
  } else if(contdata[0].button & L_JPAD) {
    step.x = -1.f;
  }

  const float rotatedXStep = (cosCameraRot * step.x) - (sinCameraRot * step.y);
  const float rotatedYStep = (sinCameraRot * step.x) + (cosCameraRot * step.y);
  playerPosition.x += rotatedXStep * PLAYER_WALK_SPEED;
  playerPosition.y += rotatedYStep * PLAYER_WALK_SPEED;

  playerPosition.x = clamp(playerPosition.x, 0.f, (float)BOARD_WIDTH);
  playerPosition.y = clamp(playerPosition.y, 0.f, (float)BOARD_WIDTH);
}

void updateGame00(void)
{ 
  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  
  updatePlayerInput();

}
