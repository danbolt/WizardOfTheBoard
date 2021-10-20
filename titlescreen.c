
#include "titlescreen.h"

#include <nusys.h>

#include "main.h"
#include "graphic.h"
#include "segmentinfo.h"

#include "opening/envtexture.h"
#include "opening/tower.h"
#include "opening/ground.h"

static float timePassed;

static u8 downPressed;
static u8 upPressed;
static u8 stickInDeadzone;

void initTitleScreen() {
  timePassed = 0.f;

  downPressed = 0;
  upPressed = 0;
  stickInDeadzone = 0;

  nuPiReadRom((u32)_opening_environmentSegmentRomStart, environmentTexture, TMEM_SIZE_BYTES);
}

void makeTitleScreenDL() {
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

  guPerspective(&dynamicp->projection, &perspectiveNorm, ingameFOV, ((float)SCREEN_WD)/((float)SCREEN_HT), 0.1f, 100.f, 1.f);
  guLookAt(&dynamicp->camera, 300.f * sinf(timePassed), -300.f * cosf(timePassed), 150.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
  
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetTexturePersp(glistp++, G_TP_PERSP);
  gDPSetTextureFilter(glistp++, G_TF_POINT);
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
  gDPLoadTextureBlock(glistp++, environmentTexture, G_IM_FMT_IA, G_IM_SIZ_8b, 128, 32, 0, G_TX_MIRROR, G_TX_MIRROR, 7, 5, G_TX_NOLOD, G_TX_NOLOD);
  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPPerspNormalize(glistp++, perspectiveNorm);
  gSPClipRatio(glistp++, FRUSTRATIO_6);

  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(gfx_Ground_None));

  gDPPipeSync(glistp++);
  gDPSetRenderMode(glistp++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
  gSPSetGeometryMode(glistp++,G_SHADE | G_ZBUFFER | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(gfx_Tower_None));

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  nuDebConClear(0);
  nuDebConTextPos(0,4,4);
  sprintf(conbuf,"title screen");
  nuDebConCPuts(0, conbuf);
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateTitleScreen() {
  nuContDataGetEx(contdata,0);

  timePassed += deltaTimeSeconds;

  if(contdata[0].trigger & U_JPAD) {
    upPressed = 1;
  } else if(contdata[0].trigger & D_JPAD) {
    downPressed = 1;
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
    upPressed = 0;
  }
  if (downPressed) {
    downPressed = 1;
  }

  if (contdata[0].trigger & A_BUTTON) {
    nextStage = &levelSelectStage;
    changeScreensFlag = 1;
  }
}
