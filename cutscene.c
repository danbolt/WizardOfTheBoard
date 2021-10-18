
#include "levelselect.h"

#include <nusys.h>

#include "main.h"
#include "graphic.h"
#include "segmentinfo.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

void initCutscene() {
  //
}

void makeCutsceneDisplaylist() {
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

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  nuDebConClear(0);
  nuDebConTextPos(0, 2, 2);
  sprintf(conbuf,"cutscene");
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0, 2, 4);
  sprintf(conbuf," audio heap: 0x%08x", NU_AU_HEAP_ADDR);
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0, 2, 5);
  sprintf(conbuf,"our bss end: 0x%08x", _codeSegmentBssEnd);
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0, 2, 6);
  sprintf(conbuf,"  remaining: 0d%08u", (u32)(NU_AU_HEAP_ADDR) - (u32)(_codeSegmentBssEnd));
  nuDebConCPuts(0, conbuf);
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateCutscene() {
  nuContDataGetEx(contdata,0);

  if (contdata[0].trigger & START_BUTTON) {
    nextStage = &levelSelectStage;
    changeScreensFlag = 1;
  }
}