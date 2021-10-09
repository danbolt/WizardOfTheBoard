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

static Pos2 chessboardSpotHighlighted;

#define VERTS_PER_FLOOR_TILE 4
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 8
#define NUMBER_OF_BOARD_CELLS (BOARD_WIDTH * BOARD_HEIGHT)
#define NUMBER_OF_FLOOR_VERTS (NUMBER_OF_BOARD_CELLS * VERTS_PER_FLOOR_TILE)
static Vtx floorVerts[NUMBER_OF_FLOOR_VERTS];

#define INV_BOARD_WIDTH (1.f / (float)BOARD_WIDTH)
#define INV_BOARD_HEIGHT (1.f / (float)BOARD_HEIGHT)

#define VERT_BUFFER_SIZE 64

#define COMMANDS_END_DL_SIZE 1
static Gfx floorDL[(NUMBER_OF_BOARD_CELLS * 2) + COMMANDS_END_DL_SIZE];

// copied from:
// https://gamedev.stackexchange.com/questions/44979/elegant-solution-for-coloring-chess-tiles
int tileIsLight(int x, int y) {
  return (x % 2) == (y % 2);
}

int tileIsDark(int x, int y) {
  return (x % 2) == (y % 2);
}

// TODO: let us customize/randomize the textures for this on init time
void generateFloorTiles() {
  Gfx* commands = floorDL;
  Vtx* verts = floorVerts;
  Vtx* lastLoad = verts;

  for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
    const int x = (i % BOARD_WIDTH);
    const int y = (i / BOARD_WIDTH);

    if (tileIsDark(x, y)) {
      *(verts++) = (Vtx){ x + 0, y + 0,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 0,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 1,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 0, y + 1,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
    } else {
      *(verts++) = (Vtx){ x + 0, y + 0,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 0,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 1,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 0, y + 1,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
    }

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

#define HUD_CELL_WIDTH 16
#define HUD_CELL_HEIGHT 10

#define HUD_CHESSBOARD_WIDTH (HUD_CELL_WIDTH * BOARD_WIDTH)
#define HUD_CHESSBOARD_HEIGHT (BOARD_HEIGHT * HUD_CELL_HEIGHT)
#define HUD_CHESSBOARD_X (SCREEN_WD - HUD_CHESSBOARD_WIDTH - TITLE_SAFE_HORIZONTAL)
#define HUD_CHESSBOARD_Y (SCREEN_HT - HUD_CHESSBOARD_HEIGHT - TITLE_SAFE_VERTICAL)


static Vtx onscreenChessboardVerts[NUMBER_OF_FLOOR_VERTS];
static Gfx onscreenChessboardCommands[(NUMBER_OF_BOARD_CELLS * 2) + COMMANDS_END_DL_SIZE];

void generateHUDChessboard() {
  Gfx* commands = onscreenChessboardCommands;
  Vtx* verts = onscreenChessboardVerts;
  Vtx* lastLoad = verts;

  for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
    const int x = ((i % BOARD_WIDTH) * HUD_CELL_WIDTH) + HUD_CHESSBOARD_X;
    const int y = HUD_CHESSBOARD_HEIGHT - ((i / BOARD_WIDTH) * HUD_CELL_HEIGHT) + HUD_CHESSBOARD_Y;

    if (tileIsDark(i % BOARD_WIDTH, i / BOARD_WIDTH)) {
      *(verts++) = (Vtx){ x + 0             , y + 0              ,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + HUD_CELL_WIDTH, y + 0              ,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + HUD_CELL_WIDTH, y - HUD_CELL_HEIGHT,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 0             , y - HUD_CELL_HEIGHT,  0, 0, 0, 0, 0x11, 0x11, 0x11, 0xff };
    } else {
      *(verts++) = (Vtx){ x + 0             , y + 0              ,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + HUD_CELL_WIDTH, y + 0              ,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + HUD_CELL_WIDTH, y - HUD_CELL_HEIGHT,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 0             , y - HUD_CELL_HEIGHT,  0, 0, 0, 0, 0xff, 0xff, 0xff, 0xff };
    }

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

#define ASCII_START_CAPTIALS 65
#define ASCCI_START_NUMBERS 48

void boardPosToLetter(register const Pos2* spot, register char* x, register char* y) {
  *x = (char)(spot->x + ASCII_START_CAPTIALS);
  *y = (char)(spot->y + 1 + ASCCI_START_NUMBERS);
}

/* The initialization of stage 0 */
void initStage00(void)
{
  generateFloorTiles();
  generateHUDChessboard();

  playerPosition = (Vec2){ 0.f, 0.f };
  playerOrientation = 0.f;
  cosCameraRot = 1.f;
  sinCameraRot = 0.f;

  chessboardSpotHighlighted = (Pos2){ 2, 2 };
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

  guOrtho(&dynamicp->ortho, 0.f, SCREEN_WD, SCREEN_HT, 0.f, 1.0F, 10.0F, 1.0F);
  guPerspective(&dynamicp->projection, &perspectiveNorm, ingameFOV, ((float)SCREEN_WD)/((float)SCREEN_HT), 0.3f, 100.f, 1.f);
  guLookAt(&dynamicp->camera, playerPosition.x, playerPosition.y, PLAYER_HEIGHT_ABOVE_GROUND, playerPosition.x - sinCameraRot, playerPosition.y + cosCameraRot, PLAYER_HEIGHT_ABOVE_GROUND, 0.f, 0.f, 1.f);
  guMtxIdent(&dynamicp->modelling);

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);
  gSPClipRatio(glistp++, FRUSTRATIO_6);

  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(floorDL));


  // drawing the HUD
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->ortho)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modelling)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(onscreenChessboardCommands));

  // TODO: make this a nice texture
  {
    const unsigned int playerHUDXPos = playerPosition.x * INV_BOARD_WIDTH * HUD_CHESSBOARD_WIDTH + HUD_CHESSBOARD_X;
    const unsigned int playerHUDYPos = (BOARD_HEIGHT - playerPosition.y) * INV_BOARD_HEIGHT * HUD_CHESSBOARD_HEIGHT + HUD_CHESSBOARD_Y;
    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);

    gDPSetFillColor(glistp++, GPACK_RGBA5551( 0, 0, 255,1)<<16 | GPACK_RGBA5551( 0, 0, 255,1));
    gDPFillRectangle(glistp++, HUD_CHESSBOARD_X + (chessboardSpotHighlighted.x * HUD_CELL_WIDTH), HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - 1 - chessboardSpotHighlighted.y) * HUD_CELL_HEIGHT), HUD_CHESSBOARD_X + (chessboardSpotHighlighted.x * HUD_CELL_WIDTH) + HUD_CELL_WIDTH, HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - 1 - chessboardSpotHighlighted.y) * HUD_CELL_HEIGHT) + HUD_CELL_HEIGHT);
    
    gDPPipeSync(glistp++);
    gDPSetFillColor(glistp++, GPACK_RGBA5551(255,180,0,1)<<16 | GPACK_RGBA5551(255,180,0,1));
    gDPFillRectangle(glistp++, playerHUDXPos - 1, playerHUDYPos - 1, playerHUDXPos + 1, playerHUDYPos + 1);
    
  }


  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      char x = 0;
      char y = 0;
      boardPosToLetter(&chessboardSpotHighlighted, &x, &y);

      /* Change character representation positions */
      nuDebConTextPos(0,4,4);
      sprintf(conbuf,"%c, %c", x, y);
      nuDebConCPuts(0, conbuf);

      /* Change character representation positions */
      nuDebConTextPos(0,2,20);
      sprintf(conbuf,"%d,%d", (glistp - gfx_glist[gfx_gtask_no]), GFX_GLIST_LEN);
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
  playerPosition.y = clamp(playerPosition.y, 0.f, (float)BOARD_HEIGHT);

  if(contdata[0].trigger & U_CBUTTONS) {
    chessboardSpotHighlighted.y = (chessboardSpotHighlighted.y + 1) % BOARD_HEIGHT;
  } else if(contdata[0].trigger & D_CBUTTONS) {
    chessboardSpotHighlighted.y = (chessboardSpotHighlighted.y - 1 + BOARD_HEIGHT) % BOARD_HEIGHT;
  }

  if(contdata[0].trigger & R_CBUTTONS) {
    chessboardSpotHighlighted.x = (chessboardSpotHighlighted.x + 1) % BOARD_WIDTH;
  } else if(contdata[0].trigger & L_CBUTTONS) {
    chessboardSpotHighlighted.x = (chessboardSpotHighlighted.x - 1 + BOARD_WIDTH) % BOARD_WIDTH;
  }
}

void updateGame00(void)
{ 
  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  
  updatePlayerInput();

}
