

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "constants.h"

#define BUFFER_COUNT 3

#define BLENDER_EXPORT_MODEL_SCALE 0.005f

/* The screen size  */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* The maximum length of the display list of one task  */
#define GFX_GLIST_LEN     2048

/* The title safe area */
#define TITLE_SAFE_HORIZONTAL 22
#define TITLE_SAFE_VERTICAL 16

/* The action safe area */
#define ACTION_SAFE_HORIZONTAL 16
#define ACTION_SAFE_VERTICAL 16

#define TMEM_SIZE_BYTES 4096

#define N64_C_BUTTONS_RED 0xFC
#define N64_C_BUTTONS_GREEN 0xd3
#define N64_C_BUTTONS_BLUE 0x17

#define N64_A_BUTTON_RED 0x02
#define N64_A_BUTTON_GREEN 0x3b
#define N64_A_BUTTON_BLUE 0xa2

/*-------------------------- define structure ------------------------------ */
/* The structure of the projection-matrix  */
typedef struct {
  Mtx ortho;
  Mtx projection;

  Mtx modelling;

  Mtx camera;

  Mtx blenderExportScale;

  Mtx cursorTranslate;
  Mtx cursorRotate;

  Mtx pieceTransforms[MAX_NUMBER_OF_INGAME_PIECES];

  Mtx puzzleSpaceScale;
  Mtx puzzleSpaceRotation;
  Mtx puzzleSpaceTranslations[MAX_NUMBER_OF_PUZZLE_SPACES];

  Mtx customTransforms[MAX_NUMBER_OF_INGAME_MONSTERS];
  Mtx monsterTranslations[MAX_NUMBER_OF_INGAME_MONSTERS];
  Mtx monsterRotations[MAX_NUMBER_OF_INGAME_MONSTERS];

  Mtx projectileTranslations[NUMBER_OF_PROJECTILES];
  Mtx projectileRotations[NUMBER_OF_PROJECTILES];

  Mtx playerFOVTranslate;
  Mtx playerFOVRotate;
} Dynamic;

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



