
#include "levelselect.h"

#include <nusys.h>

#include "main.h"
#include "graphic.h"
#include "segmentinfo.h"
#include "stagekeys.h"
#include "audio/sfx/sfx.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

static u8 backgroundTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

static u32 currentlySelectedLevel;

static float timePassed;

static u8 downPressed;
static u8 upPressed;
static u8 stickInDeadzone;

void initLevelSelect() {
  currentlySelectedLevel = 0;
  timePassed = 0.f;

  downPressed = 0;
  upPressed = 0;
  stickInDeadzone = 0;

  nuPiReadRom((u32)_level_select_backgroundSegmentRomStart, backgroundTexture, TMEM_SIZE_BYTES);
}

void makeLevelSelectDisplayList() {
  Dynamic* dynamicp;
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  guOrtho(&dynamicp->ortho, 0.f, SCREEN_WD, SCREEN_HT, 0.f, 1.0F, 10.0F, 1.0F);

  guMtxIdent(&dynamicp->modelling);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPSetTextureFilter(glistp++, G_TF_POINT);
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);

  gDPPipeSync(glistp++);
  gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(backgroundTexture), G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
  gDPSetPrimColor(glistp++, 0, 0, 0x00, 0x33, 0x61, 0xff);
  gSPTextureRectangle(glistp++, (0) << 2, (0) << 2, (SCREEN_WD) << 2, (SCREEN_HT) << 2, 0, ((u32)(timePassed * 1.f * 64.f)) << 5, ((u32)(timePassed * 0.7f * 64.f)) << 5, 1 << 10, 1 << 10);


  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  nuDebConClear(0);
  nuDebConTextPos(0,4,4);
  sprintf(conbuf,"level: %0u", (currentlySelectedLevel + 1));
  nuDebConCPuts(0, conbuf);
  nuDebConTextPos(0,4,5);
  sprintf(conbuf,"%s", levels[currentlySelectedLevel].levelKey);
  nuDebConCPuts(0, conbuf);
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateLevelSelect() {
  nuContDataGetEx(contdata,0);

  timePassed += deltaTimeSeconds;

  // I know this is kind of "backwards" but my hubris/laziness prevents me from addressing it 
  if(contdata[0].trigger & U_JPAD) {
    downPressed = 1;
  } else if(contdata[0].trigger & D_JPAD) {
    upPressed = 1;
  } else {
    upPressed = 0;
    downPressed = 0;
  }

  if (!stickInDeadzone && (contdata[0].stick_y > -7) && (contdata[0].stick_y < 7)) {
    stickInDeadzone = 1;
  }

  if (stickInDeadzone) {
    if (contdata[0].stick_y < -7) {
      upPressed = 1;
      stickInDeadzone = 0;
    } else if (contdata[0].stick_y > 7) {
      downPressed = 1;
      stickInDeadzone = 0;
    }

  }


  if (upPressed) {
    currentlySelectedLevel = (currentlySelectedLevel - 1 + NUMBER_OF_LEVELS) % NUMBER_OF_LEVELS;
    upPressed = 0;

    nuAuSndPlayerPlay(SFX_02_NOBODY_BIP);
  }
  if (downPressed) {
    currentlySelectedLevel = (currentlySelectedLevel + 1) % NUMBER_OF_LEVELS;
    downPressed = 1;

    nuAuSndPlayerPlay(SFX_02_NOBODY_BIP);
  }

  if (contdata[0].trigger & A_BUTTON) {
    currentLevel = currentlySelectedLevel;
    nextStage = &gameplayStage;
    changeScreensFlag = 1;
  }
}
