#include <assert.h>
#include <nusys.h>

#include "main.h"
#include "gamemath.h"
#include "graphic.h"
#include "tracknumbers.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

#define PLAYER_HEIGHT_ABOVE_GROUND 0.34f

static Vec2 playerPosition;
static float playerOrientation;

/* The initialization of stage 0 */
void initStage00(void)
{
  playerPosition = (Vec2){ 0.f, 0.f };
  playerOrientation = 0.f;
}

static Vtx shade_vtx[] =  {
        {         0,  5,  1, 0, 0, 0, 0, 0xff, 0, 0xff  },
        {         1,  5,  1, 0, 0, 0, 0, 0, 0, 0xff },
        {         1,  5,  0, 0, 0, 0, 0, 0, 0xff, 0xff  },
        {         0,  5,  0, 0, 0, 0, 0xff, 0, 0, 0xff  },
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

  // This is used for `gSPPerspNormalize` 
  u16 perspectiveNorm = 0;

  guOrtho(&dynamicp->ortho, -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F, -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F, 1.0F, 10.0F, 1.0F);
  guPerspective(&dynamicp->projection, &perspectiveNorm, ingameFOV, ((float)SCREEN_WD)/((float)SCREEN_HT), 0.3f, 100.f, 1.f);
  guLookAt(&dynamicp->camera, playerPosition.x, playerPosition.y, PLAYER_HEIGHT_ABOVE_GROUND, playerPosition.x, playerPosition.y + 1, PLAYER_HEIGHT_ABOVE_GROUND, 0.f, 0.f, 1.f);
  guMtxIdent(&dynamicp->modelling);

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );
  //gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modelling)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);

  gSPVertex(glistp++,&(shade_vtx[0]),4, 0);
  gSP2Triangles(glistp++,0,1,2,0,0,2,3,0);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
      nuDebConTextPos(0,4,4);
      sprintf(conbuf,"x: %2.2f, y:%2.2f", playerPosition.x, playerPosition.y);
      nuDebConCPuts(0, conbuf);

      // nuDebConTextPos(0,12,24);
      // sprintf(conbuf,"inp :%d", contdata[0].button & A_BUTTON ? 1 : 0);
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
  gfx_gtask_no ^= 1;
}

void updateGame00(void)
{ 
  /* Data reading of controller 1 */
  nuContDataGetEx(contdata,0);

  // A button poll
  if(contdata[0].trigger & A_BUTTON)
    {
      // nuAuSeqPlayerStop(0);
      // nuAuSeqPlayerSetNo(0, TRACK_1_DRUMS );
      // nuAuSeqPlayerPlay(0);
    }

}
