#include "splashscreen.h"

#include <nusys.h>

#include "audio/sfx/sfx.h"
#include "displaytext.h"
#include "gameaudio.h"
#include "graphic.h"
#include "main.h"

#define SPLASH_SCREEN_DURATION 5.f
#define BUFFER_TIME 0.75f

static float splashScreenTime;

static u8 hasPlayedLoadingSound;

const char* loadingText = "DISC READ ERR.";

void initSplashScreen() {
  splashScreenTime = 0.f;
  hasPlayedLoadingSound = 0;
}

void updateSplashScreen() {
  splashScreenTime += deltaTimeSeconds;

  if (splashScreenTime > SPLASH_SCREEN_DURATION) {
    changeScreensFlag = 1;
    nextStage = &titleScreenStage;
  }

  if (!hasPlayedLoadingSound && (splashScreenTime > BUFFER_TIME)) {
    hasPlayedLoadingSound = 1;
    playSound(SFX_31_N64_NOISES); // :wink-kiss-emoji:
  } 
}

void makeSplashScreenDL() {
  Dynamic* dynamicp;

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
  gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);


  if ((splashScreenTime > BUFFER_TIME) && (splashScreenTime < (SPLASH_SCREEN_DURATION - BUFFER_TIME))) {
    gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(displayTextTexture), G_IM_FMT_IA, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    renderDisplayText(SCREEN_WD - TITLE_SAFE_HORIZONTAL - measureDisplayText(loadingText), SCREEN_HT - TITLE_SAFE_VERTICAL - 24 + (int)(4.f * sinf(splashScreenTime * 4.f)), loadingText);
  }

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

