#include <assert.h>
#include <nusys.h>

#include "constants.h"
#include "main.h"
#include "gamemath.h"
#include "graphic.h"
#include "tracknumbers.h"
#include "segmentinfo.h"
#include "board.h"
#include "pieces.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

#define PLAYER_HEIGHT_ABOVE_GROUND 0.34f
#define PLAYER_WALK_SPEED 5.f
#define PLAYER_TURN_SPEED 2.f

#define PLAYER_MAX_HEALTH 5
#define INV_MAX_HEALTH (1.f / PLAYER_MAX_HEALTH)

#define CHESS_PIECE_RADIUS 1.f
#define CHESS_PIECE_RADIUS_SQ (CHESS_PIECE_RADIUS * CHESS_PIECE_RADIUS)

#define KNOCKBACK_SPEED 7.5f
#define KNOCKBACK_TIME 0.216f

#define PLAYER_RADIUS 0.5f

static Vec2 playerPosition;
static Vec2 playerVelocity;
static float playerOrientation;
static int playerHealth;
static u8 isKnockingBack;
static float knockbackTimeRemaining;
static float radiusSquared;

static float playerHealthDisplay;

static float cosCameraRot;
static float sinCameraRot;

#define BOARD_CONTROL_NO_SELECTED 0
#define BOARD_CONTROL_PIECE_SELECTED 1
static u32 boardControlState;
static u8 legalDestinationState[NUMBER_OF_BOARD_CELLS];

static Pos2 chessboardSpotHighlighted;

static int selectedPiece;

#define VERTS_PER_FLOOR_TILE 4
#define NUMBER_OF_FLOOR_VERTS (NUMBER_OF_BOARD_CELLS * VERTS_PER_FLOOR_TILE)
static Vtx floorVerts[NUMBER_OF_FLOOR_VERTS];

static u8 floorTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

static u8 hudIconsTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

static u8 hudNoiseBackgroundsTextre[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

static u8 displayTextTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

#define INV_BOARD_WIDTH (1.f / (float)BOARD_WIDTH)
#define INV_BOARD_HEIGHT (1.f / (float)BOARD_HEIGHT)

#define VERT_BUFFER_SIZE 64

#define COMMANDS_END_DL_SIZE 1
static Gfx floorDL[(NUMBER_OF_BOARD_CELLS * 2) + COMMANDS_END_DL_SIZE];


static Vtx wallVerts[((BOARD_WIDTH * 2) + (BOARD_HEIGHT * 2)) * VERTS_PER_FLOOR_TILE];
static Gfx wallDL[(BOARD_WIDTH * 2) + (BOARD_HEIGHT * 2) + 4 + COMMANDS_END_DL_SIZE];

const char* highlightedPieceText;

// TODO: let us customize/randomize the textures for this on init time
void generateFloorTiles() {
  Gfx* commands = floorDL;
  Vtx* verts = floorVerts;
  Vtx* lastLoad = verts;

  gDPSetCombineMode(commands++, G_CC_MODULATEI, G_CC_MODULATEI);
  gDPSetRenderMode(commands++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gDPLoadTextureBlock(commands++,  OS_K0_TO_PHYSICAL(floorTexture), G_IM_FMT_I, G_IM_SIZ_8b, 128, 32, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  gDPPipeSync(commands++);
  gSPTexture(commands++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);

  for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
    const int x = (i % BOARD_WIDTH);
    const int y = (i / BOARD_WIDTH);

    if (tileIsDark(x, y)) {
      *(verts++) = (Vtx){ x + 0, y + 0,  0, 0, 32 << 5,  0 << 5, 0xff, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 0,  0, 0, 64 << 5,  0 << 5, 0xff, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 1,  0, 0, 64 << 5, 32 << 5, 0xff, 0x11, 0x11, 0xff };
      *(verts++) = (Vtx){ x + 0, y + 1,  0, 0, 32 << 5, 32 << 5, 0xff, 0x11, 0x11, 0xff };
    } else {
      *(verts++) = (Vtx){ x + 0, y + 0,  0, 0,  0 << 5,  0 << 5, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 0,  0, 0, 32 << 5,  0 << 5, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 1, y + 1,  0, 0, 32 << 5, 32 << 5, 0xff, 0xff, 0xff, 0xff };
      *(verts++) = (Vtx){ x + 0, y + 1,  0, 0,  0 << 5, 32 << 5, 0xff, 0xff, 0xff, 0xff };
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

#define WALL_HEIGHT 1

// TODO: let us customize/randomize the textures for this on init time
void generateWalls() {
  Gfx* commands = wallDL;
  Vtx* verts = wallVerts;
  Vtx* lastLoad = verts;

  for (int i = 0; i < BOARD_WIDTH; i++) {
    *(verts++) = (Vtx){ i + 0, 0,  0, 0,  64 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ i + 1, 0,  0, 0,  96 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ i + 1, 0,  WALL_HEIGHT, 0,  96 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ i + 0, 0,  WALL_HEIGHT, 0,  64 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
  }
  gSPVertex(commands++, &(lastLoad[0]), (BOARD_WIDTH * 4), 0);
  for (int j = 0; j < (BOARD_WIDTH * 4); j += 4) {
    gSP2Triangles(commands++, j + 0, j + 1, j + 2, 0, j + 0, j + 2, j + 3, 0);
  }
  lastLoad = verts;

  for (int i = 0; i < BOARD_WIDTH; i++) {
    *(verts++) = (Vtx){ i + 0, BOARD_HEIGHT,  0, 0,  64 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ i + 1, BOARD_HEIGHT,  0, 0,  96 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ i + 1, BOARD_HEIGHT,  WALL_HEIGHT, 0,  96 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ i + 0, BOARD_HEIGHT,  WALL_HEIGHT, 0,  64 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
  }
  gSPVertex(commands++, &(lastLoad[0]), (BOARD_WIDTH * 4), 0);
  for (int j = 0; j < (BOARD_WIDTH * 4); j += 4) {
    gSP2Triangles(commands++, j + 0, j + 1, j + 2, 0, j + 0, j + 2, j + 3, 0);
  }
  lastLoad = verts;

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    *(verts++) = (Vtx){ 0, i + 0,  0, 0,  64 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ 0, i + 1,  0, 0,  96 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ 0, i + 1,  WALL_HEIGHT, 0,  96 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ 0, i + 0,  WALL_HEIGHT, 0,  64 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
  }
  gSPVertex(commands++, &(lastLoad[0]), (BOARD_HEIGHT * 4), 0);
  for (int j = 0; j < (BOARD_HEIGHT * 4); j += 4) {
    gSP2Triangles(commands++, j + 0, j + 1, j + 2, 0, j + 0, j + 2, j + 3, 0);
  }
  lastLoad = verts;

  for (int i = 0; i < BOARD_HEIGHT; i++) {
    *(verts++) = (Vtx){ BOARD_WIDTH, i + 0,  0, 0,  64 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ BOARD_WIDTH, i + 1,  0, 0,  96 << 5,  0 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ BOARD_WIDTH, i + 1,  WALL_HEIGHT, 0,  96 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
    *(verts++) = (Vtx){ BOARD_WIDTH, i + 0,  WALL_HEIGHT, 0,  64 << 5, 32 << 5, 0x46, 0x55, 0x6b, 0xff };
  }
  gSPVertex(commands++, &(lastLoad[0]), (BOARD_HEIGHT * 4), 0);
  for (int j = 0; j < (BOARD_HEIGHT * 4); j += 4) {
    gSP2Triangles(commands++, j + 0, j + 1, j + 2, 0, j + 0, j + 2, j + 3, 0);
  }
  lastLoad = verts;

  gSPEndDisplayList(commands++);
}


static Vtx HUDBackgroundVerts[] = {
  {             ACTION_SAFE_HORIZONTAL,                    0,  0, 0,               ACTION_SAFE_HORIZONTAL << 5,  0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD - ACTION_SAFE_HORIZONTAL,                    0,  0, 0, (SCREEN_WD - ACTION_SAFE_HORIZONTAL) << 5,  0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD - ACTION_SAFE_HORIZONTAL, ACTION_SAFE_VERTICAL,  0, 0, (SCREEN_WD - ACTION_SAFE_HORIZONTAL) << 5, (ACTION_SAFE_VERTICAL) << 5, 0x1d, 0x61, 0x50, 0xff },
  {             ACTION_SAFE_HORIZONTAL, ACTION_SAFE_VERTICAL,  0, 0,               ACTION_SAFE_HORIZONTAL << 5, (ACTION_SAFE_VERTICAL) << 5, 0x1d, 0x61, 0x50, 0xff },

  {                      0,                0,  0, 0,  0 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { ACTION_SAFE_HORIZONTAL,                0,  0, 0, 16 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { ACTION_SAFE_HORIZONTAL,        SCREEN_HT,  0, 0, 16 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },
  {                      0,        SCREEN_HT,  0, 0,  0 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },

  { SCREEN_WD - ACTION_SAFE_HORIZONTAL,                0,  0, 0,  0 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD                         ,                0,  0, 0, 16 << 5,         0 << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD                         ,        SCREEN_HT,  0, 0, 16 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD - ACTION_SAFE_HORIZONTAL,        SCREEN_HT,  0, 0,  0 << 5, SCREEN_HT << 5, 0x1d, 0x61, 0x50, 0xff },

  {         0,   SCREEN_HT - 80,  0, 0,         0 << 5,         (SCREEN_HT - 80) << 5, 0x1d, 0x61, 0x50, 0xff },
  { SCREEN_WD,   SCREEN_HT - 80,  0, 0, SCREEN_WD << 5,         (SCREEN_HT - 80) << 5, 0x1d, 0x61, 0x50, 0xff },
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
      *(verts++) = (Vtx){ x + 0             , y + 0              ,  0, 0, 0, 0, 0x11, 0x11, 0x50, 0xff };
      *(verts++) = (Vtx){ x + HUD_CELL_WIDTH, y + 0              ,  0, 0, 0, 0, 0x11, 0x11, 0x50, 0xff };
      *(verts++) = (Vtx){ x + HUD_CELL_WIDTH, y - HUD_CELL_HEIGHT,  0, 0, 0, 0, 0x11, 0x11, 0x50, 0xff };
      *(verts++) = (Vtx){ x + 0             , y - HUD_CELL_HEIGHT,  0, 0, 0, 0, 0x11, 0x11, 0x50, 0xff };
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

static Vtx playerFOVHUDVerts[] = {
  {  0, -20,  0, 0, 128 << 5,  0 << 5, 0xff, 0xff, 0xff, 0xff },
  { 20, -20,  0, 0, 144 << 5,  0 << 5, 0xff, 0xff, 0xff, 0xff },
  { 20,  20,  0, 0, 144 << 5, 16 << 5, 0xff, 0xff, 0xff, 0xff },
  {  0,  20,  0, 0, 128 << 5, 16 << 5, 0xff, 0xff, 0xff, 0xff },
};

void loadInTextures() {
  nuPiReadRom((u32)(_hud_iconsSegmentRomStart), (void*)(hudIconsTexture), TMEM_SIZE_BYTES);
  nuPiReadRom((u32)(_floor_tilesSegmentRomStart), (void*)(floorTexture), TMEM_SIZE_BYTES);
  nuPiReadRom((u32)(_noise_backgroundsSegmentRomStart), (void*)(hudNoiseBackgroundsTextre), TMEM_SIZE_BYTES);
  nuPiReadRom((u32)(_display_textSegmentRomStart), (void*)(displayTextTexture), TMEM_SIZE_BYTES);
}

void initializeStartingPieces() {
  initPieceStates();

  piecesActive[0] = 1;
  piecePositions[0] = (Pos2){3, 4};
  pieceData[0].type = PAWN;
  pieceData[0].renderCommands = pawn_commands;
  pieceData[0].legalCheck = pawnLegalMove;
  pieceData[0].displayName = "PAWN";
  pieceViewPos[0] = (Vec2){ piecePositions[0].x + 0.5f, piecePositions[0].y + 0.5f };

  for (int i = 1; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    piecesActive[i] = 1;
    piecePositions[i] = (Pos2){i, i};
    pieceData[i].type = ROOK;
    pieceData[i].renderCommands = rook_commands;
    pieceData[i].legalCheck = rookLegalMove;
    pieceData[i].displayName = "ROOK";
    pieceViewPos[i] = (Vec2){ piecePositions[i].x + 0.5f, piecePositions[i].y + 0.5f };
  }

}

/* The initialization of stage 0 */
void initStage00(void)
{
  generateFloorTiles();
  generateWalls();
  generateHUDChessboard();
  loadInTextures();

  highlightedPieceText = "";

  playerPosition = (Vec2){ 0.5f, 0.5f };
  playerVelocity = (Vec2){ 0.f, 0.f };
  playerOrientation = 0.f;
  isKnockingBack = 0;
  knockbackTimeRemaining = 0.f;
  radiusSquared = PLAYER_RADIUS * PLAYER_RADIUS;

  cosCameraRot = 1.f;
  sinCameraRot = 0.f;

  playerHealth = PLAYER_MAX_HEALTH;
  playerHealthDisplay = 0.f;

  chessboardSpotHighlighted = (Pos2){ 2, 2 };
  for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
    legalDestinationState[i] = 0;
  }

  selectedPiece = -1;

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    piecePositions[i] = (Pos2){ 0, 0 };
    piecesActive[i] = 0;
  }

  initializeStartingPieces();
}

#define DISPLAY_FONT_LETTER_WIDTH 13
#define DISPLAY_FONT_LETTER_HEIGHT 16
void renderDisplayText(int x, int y, const char* text) {
  int advance = x;
  for (int i = 0; text[i] != '\0'; i++) {

    const char letter = text[i];
    u32 s = 0;
    if ((letter >= 65) && (letter <= 89)) {
      s = ((letter - 65) + 2) * DISPLAY_FONT_LETTER_WIDTH;
    } else if ((letter >= 48) && (letter <= 57)) {
      s = (((letter - 48)) * DISPLAY_FONT_LETTER_WIDTH) + 364;
    }
    gSPTextureRectangle(glistp++, (advance) << 2, (y) << 2, (advance + DISPLAY_FONT_LETTER_WIDTH) << 2, (y + DISPLAY_FONT_LETTER_HEIGHT - 1) << 2, 0, s << 5, 0 << 5, 1 << 10, 1 << 10);
    advance += DISPLAY_FONT_LETTER_WIDTH;

    if (letter == 'I') {
      advance -= 6;
    } else if (s == 0) {
      advance -= 8;
    }
  }
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

  guScale(&dynamicp->blenderExportScale, BLENDER_EXPORT_MODEL_SCALE, BLENDER_EXPORT_MODEL_SCALE, BLENDER_EXPORT_MODEL_SCALE);

  guOrtho(&dynamicp->ortho, 0.f, SCREEN_WD, SCREEN_HT, 0.f, 1.0F, 10.0F, 1.0F);
  guPerspective(&dynamicp->projection, &perspectiveNorm, ingameFOV, ((float)SCREEN_WD)/((float)SCREEN_HT), 0.3f, 100.f, 1.f);
  guLookAt(&dynamicp->camera, playerPosition.x, playerPosition.y, PLAYER_HEIGHT_ABOVE_GROUND, playerPosition.x - sinCameraRot, playerPosition.y + cosCameraRot, PLAYER_HEIGHT_ABOVE_GROUND, 0.f, 0.f, 1.f);
  guMtxIdent(&dynamicp->modelling);

  guTranslate(&dynamicp->cursorTransform, chessboardSpotHighlighted.x + 0.5f, chessboardSpotHighlighted.y + 0.5f, 0.f);

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetTexturePersp(glistp++, G_TP_PERSP);
  gDPSetTextureFilter(glistp++, G_TF_POINT);
  gDPSetRenderMode(glistp++,G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gDPPipeSync(glistp++);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH);
  gSPClipRatio(glistp++, FRUSTRATIO_6);

  // TODO: walls
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(floorDL));
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(wallDL));

  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF);

  gDPPipeSync(glistp++);
  gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
  gDPSetRenderMode(glistp++,G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
  gSPSetGeometryMode(glistp++, G_ZBUFFER);

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    guTranslate(&(dynamicp->pieceTransforms[i]), pieceViewPos[i].x, pieceViewPos[i].y, 0.f);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->pieceTransforms[i])), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&dynamicp->blenderExportScale), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(pieceData[i].renderCommands));

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
  }


  // Draw the cursor
  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->cursorTransform)), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&dynamicp->blenderExportScale), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(&(cursor_commands)));
  gSPPopMatrix(glistp++, G_MTX_MODELVIEW);

  gSPClearGeometryMode(glistp++, G_ZBUFFER);
  gDPPipeSync(glistp++);
  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);


  // drawing the HUD
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->ortho)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modelling)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  const int bgTileShift = 16;
  gDPPipeSync(glistp++);
  gDPSetCombineMode(glistp++, G_CC_MODULATEI, G_CC_MODULATEI);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPLoadTextureTile(glistp++,  OS_K0_TO_PHYSICAL(hudNoiseBackgroundsTextre), G_IM_FMT_I, G_IM_SIZ_8b, 256, 16, bgTileShift << 2, 0 << 2, (bgTileShift + 15) << 2, (15) << 2, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(renderHudBackgroundCommands));


  gDPPipeSync(glistp++);
  gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_OFF);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(onscreenChessboardCommands));
  
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(hudIconsTexture), G_IM_FMT_IA, G_IM_SIZ_8b, 256, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  gDPPipeSync(glistp++);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);

  const u32 playerHUDXPos = (playerPosition.x * INV_BOARD_WIDTH * HUD_CHESSBOARD_WIDTH + HUD_CHESSBOARD_X) - 8;
  const u32 playerHUDYPos = ((BOARD_HEIGHT - playerPosition.y) * INV_BOARD_HEIGHT * HUD_CHESSBOARD_HEIGHT + HUD_CHESSBOARD_Y) - 8;

  // Render the player's FOV
  {
    guTranslate(&(dynamicp->playerFOVTranslate), playerHUDXPos + 8, playerHUDYPos + 8, 0.f);
    guRotate(&(dynamicp->playerFOVRotate), (playerOrientation * -INV_PI * 180.f) - 90.f, 0.f, 0.f, 1.f);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->playerFOVTranslate)), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->playerFOVRotate)), G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gDPPipeSync(glistp++);
    gDPSetTexturePersp(glistp++, G_TP_PERSP);
    
    gDPSetPrimColor(glistp++, 0, 0, 0xCC, 0x77, 0x22, 0xff);
    gSPVertex(glistp++, &(playerFOVHUDVerts[0]), 4, 0);
    gSP2Triangles(glistp++, 0, 1, 2, 0, 0, 2, 3, 0);

    gDPPipeSync(glistp++);
    gDPSetTexturePersp(glistp++, G_TP_NONE);

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
  }

  // Render the piece locations on the HUD
  gDPSetPrimColor(glistp++, 0, 0, 0x99, 0x99, 0x99, 0xff);
  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    const u32 pieceHUDSpotX = HUD_CHESSBOARD_X + ((pieceViewPos[i].x - 0.5f) * HUD_CELL_WIDTH);
    const u32 pieceHUDSpotY = HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - pieceViewPos[i].y - 0.5f) * HUD_CELL_HEIGHT) - ((16 - HUD_CELL_HEIGHT) / 2);

    gSPTextureRectangle(glistp++, (pieceHUDSpotX) << 2, (pieceHUDSpotY) << 2, (pieceHUDSpotX + 16) << 2, (pieceHUDSpotY + 16) << 2, 0, ((int)(pieceData[i].type) * 16) << 5, 0 << 5, 1 << 10, 1 << 10);
  }

  // Render the player location on the HUD
  {
    gDPSetPrimColor(glistp++, 0, 0, 0x11, 0x99, 0x22, 0xff);
    gSPTextureRectangle(glistp++, (playerHUDXPos) << 2, (playerHUDYPos) << 2, (playerHUDXPos + 16) << 2, (playerHUDYPos + 16) << 2, 0, 112 << 5, 0 << 5, 1 << 10, 1 << 10);
    gDPSetPrimColor(glistp++, 0, 0, 0xAC, 0x84, 0x40, 0xff);
    gSPTextureRectangle(glistp++, (playerHUDXPos) << 2, (playerHUDYPos) << 2, (playerHUDXPos + 16) << 2, (playerHUDYPos + 16) << 2, 0,  96 << 5, 0 << 5, 1 << 10, 1 << 10);
  }

  if (boardControlState == BOARD_CONTROL_PIECE_SELECTED) {
    gDPSetPrimColor(glistp++, 0, 0, 0xff, 0x00, 0x00, 0xff);
    for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
      if (!(legalDestinationState[i])) {
        const u32 noSpotX = HUD_CHESSBOARD_X + ((i % BOARD_WIDTH) * HUD_CELL_WIDTH);
        const u32 noSpotY = HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - 1 - (i / BOARD_WIDTH)) * HUD_CELL_HEIGHT) - ((16 - HUD_CELL_HEIGHT) / 2);
        gSPTextureRectangle(glistp++, (noSpotX) << 2, (noSpotY) << 2, (noSpotX + 16) << 2, (noSpotY + 16) << 2, 0, 192 << 5, 0 << 5, 1 << 10, 1 << 10);
      }
    }
  }

  // Render the cursor's location on the HUD
  {
    const u32 highightedSpotX = HUD_CHESSBOARD_X + (chessboardSpotHighlighted.x * HUD_CELL_WIDTH);
    const u32 highightedSpotY = (HUD_CHESSBOARD_Y + ((BOARD_HEIGHT - 1 - chessboardSpotHighlighted.y) * HUD_CELL_HEIGHT)) - ((16 - HUD_CELL_HEIGHT) / 2);

    gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
    gSPTextureRectangle(glistp++, (highightedSpotX) << 2, (highightedSpotY) << 2, (highightedSpotX + 16) << 2, (highightedSpotY + 16) << 2, 0,  176 << 5, 0 << 5, 1 << 10, 1 << 10);
  }

  gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(displayTextTexture), G_IM_FMT_IA, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  
  char cursorSpotString[] = { '\0', '\0', '\0'};
  boardPosToLetter(&chessboardSpotHighlighted, &(cursorSpotString[0]), &(cursorSpotString[1]));
  gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
  renderDisplayText(HUD_CHESSBOARD_X - (27), HUD_CHESSBOARD_Y + 18, cursorSpotString);

  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  renderDisplayText(HUD_CHESSBOARD_X - (12 * 8), HUD_CHESSBOARD_Y + 18 + 16, highlightedPieceText);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++, G_CYC_FILL);
  gDPSetFillColor(glistp++, GPACK_RGBA5551(0x21,0,0,1) << 16 | GPACK_RGBA5551(0x21,0,0,1));
  gDPFillRectangle(glistp++, (HUD_CHESSBOARD_X - 72), (SCREEN_HT - TITLE_SAFE_VERTICAL - 16), (HUD_CHESSBOARD_X - 7), (SCREEN_HT - TITLE_SAFE_VERTICAL));
  gDPPipeSync(glistp++);
  gDPSetFillColor(glistp++, GPACK_RGBA5551(0x33,0xc0,0x22,1) << 16 | GPACK_RGBA5551(0x33,0xc0,0x22,1));
  gDPFillRectangle(glistp++, (HUD_CHESSBOARD_X - 72), (SCREEN_HT - TITLE_SAFE_VERTICAL - 16), (HUD_CHESSBOARD_X - 72) + MAX(0, playerHealthDisplay * INV_MAX_HEALTH * 65.f), (SCREEN_HT - TITLE_SAFE_VERTICAL));

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++, G_CYC_1CYCLE);
  renderDisplayText((HUD_CHESSBOARD_X - 72) + 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 16) - 4, "LIFE");

  if (playerHealth <= 0) {
    renderDisplayText(SCREEN_WD / 2 - ((5 * 13) / 2), SCREEN_HT / 2, "DEATH");
  }

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
      nuDebConTextPos(0,4,4);
      sprintf(conbuf,"DL: %04d/%04d", (glistp - gfx_glist[gfx_gtask_no]), GFX_GLIST_LEN);
      nuDebConCPuts(0, conbuf);

      
      nuDebConTextPos(0,4,5);
      sprintf(conbuf,"delta: %3.5f", deltaTimeSeconds);
      nuDebConCPuts(0, conbuf);


      // nuDebConTextPos(0,4,9);
      // sprintf(conbuf,"playerHealth: %u", playerHealth);
      // nuDebConCPuts(0, conbuf);
      // nuDebConTextPos(0,4,10);
      // sprintf(conbuf,"isKnockingBack: %u", isKnockingBack);
      // nuDebConCPuts(0, conbuf);
      // nuDebConTextPos(0,4,11);
      // sprintf(conbuf,"knockbackTimeRemaining: %3.2f", knockbackTimeRemaining);
      // nuDebConCPuts(0, conbuf);
    }
  else
    {
      nuDebConTextPos(0,9,24);
      nuDebConCPuts(0, "Controller1 not connect");
    }
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updatePlayerInput() {
  // We don't need to update this if we're dead
  if (playerHealth <= 0) {
    return;
  }

  Vec2 inputDir = { 0.f, 0.f };

  // Update rotation
  if((contdata[0].button & L_TRIG) || (contdata[0].stick_x < -7)) {
    playerOrientation += PLAYER_TURN_SPEED * deltaTimeSeconds;

    if (playerOrientation > M_PI) {
      playerOrientation = -M_PI;
    }
  } else if((contdata[0].button & R_TRIG) || (contdata[0].stick_x > 7)) {
    playerOrientation -= PLAYER_TURN_SPEED * deltaTimeSeconds;

    if (playerOrientation < -M_PI) {
      playerOrientation = M_PI;
    }
  }
  cosCameraRot = cosf(playerOrientation);
  sinCameraRot = sinf(playerOrientation);

  // We don't need to continue if we're being knocked back
  if (isKnockingBack) {
    return;
  }


  if (contdata[0].stick_y > 7) {
    inputDir.y = 1.f;
  } else if (contdata[0].stick_y < -7) {
    inputDir.y = -1.f;
  }

  // Update position
  if(contdata[0].button & U_JPAD) {
    inputDir.y = 1.f;
  } else if(contdata[0].button & D_JPAD) {
    inputDir.y = -1.f;
  }

  if(contdata[0].button & R_JPAD) {
    inputDir.x = 1.f;
  } else if(contdata[0].button & L_JPAD) {
    inputDir.x = -1.f;
  }

  const float rotatedXStep = (cosCameraRot * inputDir.x) - (sinCameraRot * inputDir.y);
  const float rotatedYStep = (sinCameraRot * inputDir.x) + (cosCameraRot * inputDir.y);
  playerVelocity.x = rotatedXStep * PLAYER_WALK_SPEED;
  playerVelocity.y = rotatedYStep * PLAYER_WALK_SPEED;
}

void updateMovement() {
  Vec2 desiredSpot = { playerPosition.x + (playerVelocity.x * deltaTimeSeconds), playerPosition.y + (playerVelocity.y * deltaTimeSeconds) };

  // step x
  if (isSpaceOccupiedButIgnoreMovingPieces((int)(desiredSpot.x), (int)(playerPosition.y)) > -1) {
    if (playerVelocity.x > 0.f) {
      desiredSpot.x = (float)((int)(desiredSpot.x) - 1) + 0.999f;
    } else if (playerVelocity.x < 0.f) {
      desiredSpot.x = (float)((int)(desiredSpot.x) + 1);
    }
    //desiredSpot.x = playerPosition.x;
  }

  // step y
  if (isSpaceOccupiedButIgnoreMovingPieces((int)(desiredSpot.x), (int)(desiredSpot.y)) > -1) {
    if (playerVelocity.y > 0.f) {
      desiredSpot.y = (float)((int)(desiredSpot.y) - 1) + 0.999f;
    } else if (playerVelocity.y < 0.f) {
      desiredSpot.y = (float)((int)(desiredSpot.y) + 1);
    }
    //desiredSpot.y = playerPosition.y;
  }

  playerPosition.x = desiredSpot.x;
  playerPosition.y = desiredSpot.y;

  // Don't let the player leave the area
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

        for (int i = 0; i < NUMBER_OF_BOARD_CELLS; i++) {
          legalDestinationState[i] = 0;
        }
        pieceData[selectedPiece].legalCheck(selectedPiece, piecesActive, piecePositions, legalDestinationState);
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

      int isSelectedSpotValid = pieceAtCursorSpot < 0;

      // If the destination isn't legal we can't place it there
      if (!(legalDestinationState[(chessboardSpotHighlighted.x % BOARD_WIDTH) + (chessboardSpotHighlighted.y * BOARD_WIDTH)])) {
        isSelectedSpotValid = 0;
      }

      if (isSelectedSpotValid) {
        oldPiecePos[selectedPiece] = (Vec2){ piecePositions[selectedPiece].x + 0.5f, piecePositions[selectedPiece].y + 0.5f };

        piecePositions[selectedPiece] = chessboardSpotHighlighted;
        pieceIsLerping[selectedPiece] = 1;
        pieceLerpValue[selectedPiece] = 0.f;

        // TODO: play a "complete" sound
      } else {
        // TODO: play a "wrong" sound
      }

      selectedPiece = -1;
      boardControlState = BOARD_CONTROL_NO_SELECTED;
    }
  }
}

void updateMovingPieces() {
  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    if (!(pieceIsLerping[i])) {
      continue;
    }

    pieceLerpValue[i] += 0.05f;

    if (pieceLerpValue[i] >= 1.f) {
      pieceLerpValue[i] = 0.f;
      pieceIsLerping[i] = 0;

      pieceViewPos[i] = (Vec2){ piecePositions[i].x + 0.5f, piecePositions[i].y + 0.5f };
    } else {

      //
      pieceViewPos[i] = (Vec2){ lerp(oldPiecePos[i].x, piecePositions[i].x + 0.5f, pieceLerpValue[i]), lerp(oldPiecePos[i].y, piecePositions[i].y + 0.5f, pieceLerpValue[i]) };
    }
  }
}

void updateHUDInformation() {
  // Update the text for the selected piece
  if (boardControlState == BOARD_CONTROL_NO_SELECTED) {
    const int pieceAtCursorSpot = isSpaceOccupied(chessboardSpotHighlighted.x, chessboardSpotHighlighted.y);
    if (pieceAtCursorSpot > -1) {
      highlightedPieceText = pieceData[pieceAtCursorSpot].displayName;
    } else {
      highlightedPieceText = "";
    }
  }

  // Lerp the player's healthbar to their health;
  playerHealthDisplay = lerp(playerHealthDisplay, playerHealth, 0.13f);
}

void checkCollisionWithPieces() {
  if (isKnockingBack) {
    return;
  }

  for (int i = 0; i < MAX_NUMBER_OF_INGAME_PIECES; i++) {
    if (!(piecesActive[i])) {
      continue;
    }

    if (!(pieceIsLerping[i])) {
      continue;
    }

    // Radius check
    const float distanceSquared = distanceSq(&playerPosition, &(pieceViewPos[i]));
    if (distanceSquared > MIN(radiusSquared, CHESS_PIECE_RADIUS_SQ)) {
      continue;
    }

    isKnockingBack = 1;
    knockbackTimeRemaining = KNOCKBACK_TIME;

    playerHealth = MAX(playerHealth - 1, 0);

    // Fly back away from the piece
    playerVelocity = (Vec2){ playerPosition.x - pieceViewPos[i].x, playerPosition.y - pieceViewPos[i].y };
    normalize(&playerVelocity);
    playerVelocity.x *= KNOCKBACK_SPEED;
    playerVelocity.y *= KNOCKBACK_SPEED;

    break;
  }
}

void updateKnockback() {
  if (!isKnockingBack) {
    return;
  }

  knockbackTimeRemaining -= deltaTimeSeconds;
  if (knockbackTimeRemaining <= 0.f) {
    isKnockingBack = 0;
    playerVelocity = (Vec2){ 0.f, 0.f };
  }
}

void updateGame00(void)
{
  nuContDataGetEx(contdata,0);

  
  updatePlayerInput();
  updateBoardControlInput();

  updateMovement();

  updateMovingPieces();

  checkCollisionWithPieces();

  updateKnockback();

  updateHUDInformation();
}
