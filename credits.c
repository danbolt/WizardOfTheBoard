#include "credits.h"

#include <nusys.h>

#include "graphic.h"
#include "main.h"
#include "sixtwelve.h"
#include "sixtwelve_helpers.h"

static float creditsTime;

void initCredits() {
  creditsTime = 0.f;
}

void writeString(int x, int y, const char* str) {
  int xSpot = x;
  int stringIndex = 0;

  while (str[stringIndex] != '\0') {
    
    const unsigned char character = str[stringIndex];
    const sixtwelve_character_info* characterInfo = sixtwelve_get_character_info(character);
    const int xLoc = xSpot + characterInfo->x_offset;
    const int yLoc = y + characterInfo->y_offset;

    gSPScisTextureRectangle(glistp++, (xLoc) << 2, (yLoc) << 2, (xLoc + characterInfo->width) << 2, (yLoc + characterInfo->height) << 2, 0, (characterInfo->x) << 5, (characterInfo->y) << 5, 1 << 10, 1 << 10);

    xSpot += characterInfo->x_advance;
    stringIndex++;
  }
}

void makeCreditsDisplaylist() {
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
  gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);

  gDPPipeSync(glistp++);
  gDPLoadTextureBlock_4b(glistp++, sixtwelve_tex, G_IM_FMT_IA, SIXTWELVE_TEXTURE_WIDTH, SIXTWELVE_TEXTURE_HEIGHT, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  writeString(32, 32, "hello, world!");

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_SWAPBUFFER);
}

void updateCredits() {
  //
}
