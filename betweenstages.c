#include "betweenstages.h"

#include "graphic.h"
#include "displaytext.h"
#include "main.h"
#include "segmentinfo.h"
#include "stagekeys.h"


static u8 walkingAnimTexture[TMEM_SIZE_BYTES * 2] __attribute__((aligned(8)));

#define NUMBER_OF_ANIMATION_FRAMES 16
static float betweenTimePassed;
static u32 animationFrame;

void initBetweenStages() {
  nuPiReadRom((u32)(_stairs_animSegmentRomStart), (void*)(walkingAnimTexture), TMEM_SIZE_BYTES * 2);

  betweenTimePassed = 0.f;
  animationFrame = 0;
}

void makeBetweenStagesDisplaylist() {
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
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);

  gDPPipeSync(glistp++);
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(walkingAnimTexture), G_IM_FMT_I, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  gSPScisTextureRectangle(glistp++, (((SCREEN_WD - 32) / 2)) << 2, (((SCREEN_HT - 32) / 2 ) + 16) << 2, (((SCREEN_WD - 32) / 2) + 32) << 2, (((SCREEN_HT - 32) / 2 ) + 16 + 16) << 2, 0, (animationFrame * 32) << 5, 0 << 5, 1 << 10, 1 << 10);
  gDPPipeSync(glistp++);
  gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(walkingAnimTexture + TMEM_SIZE_BYTES), G_IM_FMT_I, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  gSPScisTextureRectangle(glistp++, (((SCREEN_WD - 32) / 2)) << 2, (((SCREEN_HT - 32) / 2 ) + 32) << 2, (((SCREEN_WD - 32) / 2) + 32) << 2, (((SCREEN_HT - 32) / 2 ) + 32 + 16) << 2, 0, (animationFrame * 32) << 5, 0 << 5, 1 << 10, 1 << 10);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;

}

void updateBetweenStages() {
  betweenTimePassed += deltaTimeSeconds;
  animationFrame = ((int)(betweenTimePassed * 24.f)) % NUMBER_OF_ANIMATION_FRAMES;

  if (betweenTimePassed > 1.5f) {
    nextStage = &gameplayStage;
    currentLevel = (currentLevel + 1) % NUMBER_OF_LEVELS; // TODO: finish the game if we top out
    changeScreensFlag = 1;
  }
}
