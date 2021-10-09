#include <assert.h>
#include <nusys.h>

#include "constants.h"
#include "main.h"
#include "gamemath.h"
#include "graphic.h"
#include "tracknumbers.h"
#include "segmentinfo.h"

#include "pieces.h"

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


#define BOARD_CONTROL_NO_SELECTED 0
#define BOARD_CONTROL_PIECE_SELECTED 1
static u32 boardControlState;

static Pos2 chessboardSpotHighlighted;

static int selectedPiece;

static u8 piecesActive[MAX_NUMBER_OF_INGAME_PIECES];
static Pos2 piecePositions[MAX_NUMBER_OF_INGAME_PIECES];
static PieceInfo pieceData[MAX_NUMBER_OF_INGAME_PIECES];

#define VERTS_PER_FLOOR_TILE 4
#define NUMBER_OF_FLOOR_VERTS (NUMBER_OF_BOARD_CELLS * VERTS_PER_FLOOR_TILE)
static Vtx floorVerts[NUMBER_OF_FLOOR_VERTS];

static u8 hudIconsTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

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

// returns -1 if empty, otherwise the index of the occupying piece
int isSpaceOccupied(int x, int y) {

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    if (piecePositions[i].x != x) {
      continue;
    }

    if (piecePositions[i].y != y) {
      continue;
    }

    // If we've made it here, we've found an occupying piece
    return i;
  }

  return -1;
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

static Vtx HUDBackgroundVerts[] = {
  {         0,                    0,  0, 0,  0 << 5,  0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD,                    0,  0, 0, 16 << 5,  0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD, ACTION_SAFE_VERTICAL,  0, 0, 16 << 5, 16 << 5, 0x1d, 0x61, 0x50, 0xff },
  {         0, ACTION_SAFE_VERTICAL,  0, 0,  0 << 5, 16 << 5, 0x1d, 0x61, 0x50, 0xff },

  {                      0,                0,  0, 0,  0 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { ACTION_SAFE_HORIZONTAL,                0,  0, 0, 16 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { ACTION_SAFE_HORIZONTAL,        SCREEN_HT,  0, 0, 16 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },
  {                      0,        SCREEN_HT,  0, 0,  0 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },

  { SCREEN_WD - ACTION_SAFE_HORIZONTAL,                0,  0, 0,  0 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD                         ,                0,  0, 0, 16 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD                         ,        SCREEN_HT,  0, 0, 16 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD - ACTION_SAFE_HORIZONTAL,        SCREEN_HT,  0, 0,  0 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },

  {         0,   SCREEN_HT - 80,  0, 0,         0 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD,   SCREEN_HT - 80,  0, 0, SCREEN_WD << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD,        SCREEN_HT,  0, 0, SCREEN_WD << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },
  {         0,        SCREEN_HT,  0, 0,         0 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },
};

static Gfx renderHudBackgroundCommands[] = {
  gsSPVertex(HUDBackgroundVerts, 16, 0),
  gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
  gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
  gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
  gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
  gsSPEndDisplayList()
};

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

void boardPosToLetter(const Pos2* spot, char* x, char* y) {
  *x = (char)(spot->x + ASCII_START_CAPTIALS);
  *y = (char)(spot->y + 1 + ASCCI_START_NUMBERS);
}

void loadInTextures() {
  nuPiReadRom((u32)(_hud_iconsSegmentRomStart), (void*)(hudIconsTexture), TMEM_SIZE_BYTES);
}

void initializeStartingPieces() {
  piecesActive[0] = 1;
  piecePositions[0] = (Pos2){3, 4};
  pieceData[0].type = PAWN;
  pieceData[0].renderCommands = pawn_commands;

  piecesActive[1] = 1;
  piecePositions[1] = (Pos2){0, 1};
  pieceData[1].type = ROOK;
  pieceData[1].renderCommands = rook_commands;
}

/* The initialization of stage 0 */
void initStage00(void)
{
  generateFloorTiles();
  generateHUDChessboard();
  loadInTextures();

  playerPosition = (Vec2){ 0.f, 0.f };
  playerOrientation = 0.f;
  cosCameraRot = 1.f;
  sinCameraRot = 0.f;

  chessboardSpotHighlighted = (Pos2){ 2, 2 };

  selectedPiece = -1;

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    piecePositions[i] = (Pos2){ 0, 0 };
    piecesActive[i] = 0;
  }

  initializeStartingPieces();
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

  guScale(&dynamicp->blenderExportScale, 0.005f, 0.005f, 0.005f);

  guOrtho(&dynamicp->ortho, 0.f, SCREEN_WD, SCREEN_HT, 0.f, 1.0F, 10.0F, 1.0F);
  guPerspective(&dynamicp->projection, &perspectiveNorm, ingameFOV, ((float)SCREEN_WD)/((float)SCREEN_HT), 0.3f, 100.f, 1.f);
  guLookAt(&dynamicp->camera, playerPosition.x, playerPosition.y, PLAYER_HEIGHT_ABOVE_GROUND, playerPosition.x - sinCameraRot, playerPosition.y + cosCameraRot, PLAYER_HEIGHT_ABOVE_GROUND, 0.f, 0.f, 1.f);
  guMtxIdent(&dynamicp->modelling);

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPSetTextureFilter(glistp++, G_TF_POINT);
  gDPSetRenderMode(glistp++,G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);
  gSPClipRatio(glistp++, FRUSTRATIO_6);

  // TODO: walls
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(floorDL));

  gDPPipeSync(glistp++);
  gDPSetRenderMode(glistp++,G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
  gSPSetGeometryMode(glistp++, G_ZBUFFER);

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    guTranslate(&(dynamicp->pieceTransforms[i]), ((float)piecePositions[i].x) + 0.5f, ((float)piecePositions[i].y) + 0.5f, 0.f);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->pieceTransforms[i])), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&dynamicp->blenderExportScale), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(pieceData[i].renderCommands));

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
  }

  gSPClearGeometryMode(glistp++, G_ZBUFFER);
  gDPPipeSync(glistp++);
  gDPSetRenderMode(glistp++,G_RM_OPA_SURF, G_RM_OPA_SURF2);


  // drawing the HUD
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->ortho)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modelling)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );



  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(renderHudBackgroundCommands));

  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(onscreenChessboardCommands));

  
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
  gDPSetRenderMode(glistp++, G_RM_AA_TEX_EDGE, G_RM_AA_TEX_EDGE2);
  gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(hudIconsTexture), G_IM_FMT_IA, G_IM_SIZ_8b, 256, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  gDPPipeSync(glistp++);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);

  // Render the piece locations on the HUD
  gDPSetPrimColor(glistp++, 0, 0, 0x99, 0x99, 0x99, 0xff);
  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    const u32 pieceHUDSpotX = HUD_CHESSBOARD_X + (piecePositions[i].x * HUD_CELL_WIDTH);
    const u32 pieceHUDSpotY = HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - 1 - piecePositions[i].y) * HUD_CELL_HEIGHT) - ((16 - HUD_CELL_HEIGHT) / 2);

    gSPTextureRectangle(glistp++, (pieceHUDSpotX) << 2, (pieceHUDSpotY) << 2, (pieceHUDSpotX + 16) << 2, (pieceHUDSpotY + 16) << 2, 0, ((int)(pieceData[i].type) * 16) << 5, 0 << 5, 1 << 10, 1 << 10);
  }

  // Render the player location on the HUD
  {
    // TODO: render the player's FOV

    const u32 playerHUDXPos = (playerPosition.x * INV_BOARD_WIDTH * HUD_CHESSBOARD_WIDTH + HUD_CHESSBOARD_X) - 8;
    const u32 playerHUDYPos = ((BOARD_HEIGHT - playerPosition.y) * INV_BOARD_HEIGHT * HUD_CHESSBOARD_HEIGHT + HUD_CHESSBOARD_Y) - 8;

    gDPSetPrimColor(glistp++, 0, 0, 0x11, 0x11, 0x99, 0xff);
    gSPTextureRectangle(glistp++, (playerHUDXPos) << 2, (playerHUDYPos) << 2, (playerHUDXPos + 16) << 2, (playerHUDYPos + 16) << 2, 0, 112 << 5, 0 << 5, 1 << 10, 1 << 10);
    gDPSetPrimColor(glistp++, 0, 0, 0xAC, 0x84, 0x40, 0xff);
    gSPTextureRectangle(glistp++, (playerHUDXPos) << 2, (playerHUDYPos) << 2, (playerHUDXPos + 16) << 2, (playerHUDYPos + 16) << 2, 0,  96 << 5, 0 << 5, 1 << 10, 1 << 10);
  }

  // Render the cursor's location on the HUD
  {
    const u32 highightedSpotX = HUD_CHESSBOARD_X + (chessboardSpotHighlighted.x * HUD_CELL_WIDTH);
    const u32 highightedSpotY = (HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - 1 - chessboardSpotHighlighted.y) * HUD_CELL_HEIGHT)) - ((16 - HUD_CELL_HEIGHT) / 2);

    if (boardControlState == BOARD_CONTROL_NO_SELECTED) {
      gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
    } else {
      gDPSetPrimColor(glistp++, 0, 0, N64_A_BUTTON_RED, N64_A_BUTTON_GREEN, N64_A_BUTTON_BLUE, 0xff);
    }
    gSPTextureRectangle(glistp++, (highightedSpotX) << 2, (highightedSpotY) << 2, (highightedSpotX + 16) << 2, (highightedSpotY + 16) << 2, 0,  176 << 5, 0 << 5, 1 << 10, 1 << 10);
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
      sprintf(conbuf,"DL: %d,%d", (glistp - gfx_glist[gfx_gtask_no]), GFX_GLIST_LEN);
      nuDebConCPuts(0, conbuf);

      /* Change character representation positions */
      nuDebConTextPos(0,2,24);
      sprintf(conbuf,"bstate: %u", boardControlState);
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

  
}

void updateBoardControlInput() {
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

  if (boardControlState == BOARD_CONTROL_NO_SELECTED) {
    if (contdata[0].trigger & A_BUTTON) {
      const int pieceAtCursorSpot = isSpaceOccupied(chessboardSpotHighlighted.x, chessboardSpotHighlighted.y);

      if (pieceAtCursorSpot >= 0) {
        // TODO: unselectable pieces

        boardControlState = BOARD_CONTROL_PIECE_SELECTED;
        selectedPiece = pieceAtCursorSpot;
      }
    }
  } else if (boardControlState == BOARD_CONTROL_PIECE_SELECTED) {
    if (contdata[0].trigger & B_BUTTON) {
      selectedPiece = -1;
      boardControlState = BOARD_CONTROL_NO_SELECTED;
      //
    } else if (contdata[0].trigger & A_BUTTON) {
      assert(selectedPiece >= 0); // we should have a selected piece here
      const int pieceAtCursorSpot = isSpaceOccupied(chessboardSpotHighlighted.x, chessboardSpotHighlighted.y);

      // TODO: check if position valid
      int isSelectedSpotValid = pieceAtCursorSpot < 0;

      if (isSelectedSpotValid) {
        piecePositions[selectedPiece] = chessboardSpotHighlighted;

        // TODO: play a "complete" sound
      } else {
        // TODO: play a "wrong" sound
      }

      selectedPiece = -1;
      boardControlState = BOARD_CONTROL_NO_SELECTED;
    }
  }
}

void updateGame00(void)
{ 
  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  
  updatePlayerInput();
  updateBoardControlInput();
}
