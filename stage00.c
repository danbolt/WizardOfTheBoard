#include <assert.h>
#include <nusys.h>

#include "main.h"
#include "graphic.h"

void shadetri(Dynamic* dynamicp);

/* The initialization of stage 0 */
void initStage00(void)
{

}

static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0, 0, 0, 0xff, 0, 0xff  },
        {         64,  64, -5, 0, 0, 0, 0, 0, 0, 0xff },
        {         64, -64, -5, 0, 0, 0, 0, 0, 0xff, 0xff  },
        {        -64, -64, -5, 0, 0, 0, 0xff, 0, 0, 0xff  },
};


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

  guOrtho(&dynamicp->ortho, -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F, -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F, 1.0F, 10.0F, 1.0F);
  guMtxIdent(&dynamicp->modelling);

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->ortho)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modelling)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gSPVertex(glistp++,&(shade_vtx[0]),4, 0);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);

  gSP2Triangles(glistp++,0,1,2,0,0,2,3,0);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
      nuDebConTextPos(0,12,23);
      sprintf(conbuf,"Hello, world!");
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

void updateGame00(void)
{ 

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  // A button poll
  if(contdata[0].trigger & A_BUTTON)
    {
      //
    }

}

/* Draw a square */
void shadetri(Dynamic* dynamicp)
{

}
