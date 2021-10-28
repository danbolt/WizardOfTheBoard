
#include "titlescreen.h"

#include <nusys.h>

#include "gamemath.h"
#include "backgroundbuffers.h"
#include "cutscene_backgrounds/backgroundlookup.h"
#include "nustdfuncs.h"
#include "main.h"
#include "graphic.h"
#include "segmentinfo.h"
#include "sixtwelve.h"
#include "sixtwelve_helpers.h"
#include "audio/bgm/sequence/tracknumbers.h"

#include "opening/envtexture.h"
#include "opening/tower.h"
#include "opening/ground.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

static float timePassed;

static u8 downPressed;
static u8 upPressed;
static u8 stickInDeadzone;

#define OVERTURE_DURATION_SECONDS 48

typedef struct {
  float duration;
  Vec3 camera;
  Vec3 look;
  const char* text;
} Spot;

static int spotIndex;
static float spotTimePassed;

static Spot spots[] = {
  // Sky start
  { 6.f, { -99.3196f, -120.643f, 67.314f }, { -59.3667f, -69.484f, 123.734f }, "It is a shitty time." },

  // "comes down" to view
  { 6.f, { -59.3667f, -120.643f, 67.314f }, { -59.3667f, -69.484f, 67.314f }, "The harvests are poor, and\n  monsters roam freely\n   across the land." },

  { 6.5f, { 83.7952f, -115.326f, 37.987f }, { 6.07872f, 11.4772f, -9.87373f }, " Chosen by lot, warriors are\n  trained from birth\n     in the mystic artes." },
  { 6.5f, { 178.037f, 36.7114f, 76.2756f }, { 6.07872f, 19.6313f, 21.6789f }, "A warrior must be learned\n           in cunning,\n                  strategy,\n                   and swiftness..." },
  { 6.5f, { 76.7275f, 73.2299f, 76.2756f }, { 5.48971f, 23.1654f, 14.9429f }, "...for their final lonesome trial\n        when they come of age." },

  // Midpoint
  { 6.f, { 0.745361f, 44.7062f, 108.537f }, { 0.667588f, 4.2692f, 24.0872f }, "To prevail as a warrior is to\n    scale the Demon's Spire,\n     lair of the Shadow Queen." },

  { 4.f, { -103.509f, 22.3239f, 70.2484f }, { -0.489938f, 21.5849f, 22.4681f }, "Many have entered,\n but few have returned." },
  { 6.f, { -4.55552f, -93.9098f, 44.2122f }, { 1.36965f, 21.797f, 24.2866f }, "If one prevails over the trial,\n   if one succeeds at their task,\n     they shall be known as a..." },
  { 5.f, { 0.156551f, 7.43251f, 25.1426f }, { 1.36965f, 21.797f, 24.2866f }, "WIZARD OF THE BOARD" },

  { 1.f, { 0.f, -5.f, 0.f }, { 0.f, 11.5f, 26.f }, "" },
};
#define NUMBER_OF_SPOTS 9



void initializeBackgrounds() {
  struct backgroundMappingData* mapping = getBackgroundTextureOffset("stars", _nstrlen("stars"));
  if (mapping != NULL) {
    nuPiReadRom((u32)(_packedbackgroundsSegmentRomStart + mapping->offset), backgroundBuffers[0], 320 * 240 * 2);
  } else {
    bzero(backgroundBuffers[0], 320 * 240 * 2);
  }
}

void initTitleScreen() {
  timePassed = 0.f;

  downPressed = 0;
  upPressed = 0;
  stickInDeadzone = 0;

  initializeBackgrounds();
  nuPiReadRom((u32)_opening_environmentSegmentRomStart, environmentTexture, TMEM_SIZE_BYTES);

  spotIndex = 0;
  spotTimePassed = 0.f;


  // nuAuSeqPlayerStop(0);
  // nuAuSeqPlayerSetNo(0, TRACK_4_OVERTURE);
  // nuAuSeqPlayerPlay(0);
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
  if (spotIndex < (NUMBER_OF_SPOTS - 1)) {
    const float t = spotTimePassed / spots[spotIndex].duration;
    Vec3 camPos = {
      lerp(spots[spotIndex].camera.x, spots[spotIndex + 1].camera.x, t),
      lerp(spots[spotIndex].camera.y, spots[spotIndex + 1].camera.y, t),
      lerp(spots[spotIndex].camera.z, spots[spotIndex + 1].camera.z, t),
    };
    Vec3 look = {
      lerp(spots[spotIndex].look.x, spots[spotIndex + 1].look.x, t),
      lerp(spots[spotIndex].look.y, spots[spotIndex + 1].look.y, t),
      lerp(spots[spotIndex].look.z, spots[spotIndex + 1].look.z, t),
    };

    guLookAt(&dynamicp->camera, camPos.x, camPos.y, camPos.z, look.x, look.y, look.z, 0.f, 0.f, 1.f);
  } else {
    guLookAt(&dynamicp->camera, spots[NUMBER_OF_SPOTS - 1].camera.x, spots[NUMBER_OF_SPOTS - 1].camera.y, spots[NUMBER_OF_SPOTS - 1].camera.z, spots[NUMBER_OF_SPOTS - 1].look.x, spots[NUMBER_OF_SPOTS - 1].look.y, spots[NUMBER_OF_SPOTS - 1].look.z, 0.f, 0.f, 1.f);
  }


  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPSetTextureFilter(glistp++, G_TF_POINT);
  gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  for (int i = 0; i < (240 / 6); i++) {
    gDPPipeSync(glistp++);
    gDPLoadTextureTile(glistp++, backgroundBuffers[0], G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, 240, 0, (i * 6), 320 - 1, ((i + 1) * 6) - 1, 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD );
    gSPTextureRectangle(glistp++, 0 << 2, (0 + (i * 6)) << 2, (0 + 320) << 2, (0 + ((i + 1) * 6)) << 2, 0, 0 << 5, (i * 6) << 5, 1 << 10, 1 << 10);
  }

  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)), G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->camera)), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH );

  gDPPipeSync(glistp++);
  gDPSetTexturePersp(glistp++, G_TP_PERSP);
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
  gDPLoadTextureBlock(glistp++, environmentTexture, G_IM_FMT_IA, G_IM_SIZ_8b, 128, 32, 0, G_TX_MIRROR, G_TX_MIRROR, 7, 5, G_TX_NOLOD, G_TX_NOLOD);
  gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPPerspNormalize(glistp++, perspectiveNorm);
  gSPClipRatio(glistp++, FRUSTRATIO_6);

  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(gfx_Ground_None));

  gDPPipeSync(glistp++);
  gDPSetRenderMode(glistp++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
  gSPSetGeometryMode(glistp++,G_SHADE | G_ZBUFFER | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(gfx_Tower_None));

  gDPPipeSync(glistp++);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gDPLoadTextureBlock_4b(glistp++, sixtwelve_tex, G_IM_FMT_IA, SIXTWELVE_TEXTURE_WIDTH, SIXTWELVE_TEXTURE_HEIGHT, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  const char* str = NULL;

  if ((spotIndex < (NUMBER_OF_SPOTS)) && (spots[spotIndex].text != NULL) && ((spotIndex > 0) && (spotTimePassed > 1.f)) || ( spotTimePassed > 3.f) ) {
    str = spots[spotIndex].text;
  }

  if (spotIndex >= NUMBER_OF_SPOTS) {
    str = "WIZARD OF THE BOARD";
  }

  if ((str != NULL) ) {
    int i = 0;
    int xInit = 32;
    int xAdv = xInit;
    int y = 128;
    while (spots[spotIndex].text[i] != '\0') {
      const sixtwelve_character_info* characterInfo = sixtwelve_get_character_info(spots[spotIndex].text[i]);

      if (spots[spotIndex].text[i] == '\n') {
        xAdv = xInit;
        y += SIXTWELVE_LINE_HEIGHT;
        i++;
        continue;
      }

      const int xLoc = xAdv + characterInfo->x_offset;
      const int yLoc = y + characterInfo->y_offset;

      gSPScisTextureRectangle(glistp++, (xLoc) << 2, (yLoc) << 2, (xLoc + characterInfo->width) << 2, (yLoc + characterInfo->height) << 2, 0, (characterInfo->x) << 5, (characterInfo->y) << 5, 1 << 10, 1 << 10);
      xAdv += characterInfo->x_advance;
      i++;
    }
  }

#ifdef LINES_CAPTURE_MARKERS
  if (spotIndex < (NUMBER_OF_SPOTS) && spotTimePassed) {
    const float percentage = (spotTimePassed) / (spots[spotIndex].duration);

    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetFillColor(glistp++, GPACK_RGBA5551(0xc0,0,0,1) << 16 | GPACK_RGBA5551(0xc0,0,0,1));
    gDPFillRectangle(glistp++, 0, SCREEN_HT - 8, SCREEN_WD - 1, SCREEN_HT);

    gDPPipeSync(glistp++);
    gDPSetFillColor(glistp++, GPACK_RGBA5551(0x5a,0,0,1) << 16 | GPACK_RGBA5551(0x5a,0,0,1));
    gDPFillRectangle(glistp++, 0, SCREEN_HT - 8,(u32)((SCREEN_WD) * (1.f / spots[spotIndex].duration)), SCREEN_HT);

    gDPPipeSync(glistp++);
    gDPSetFillColor(glistp++, GPACK_RGBA5551(0,0xc0,0,1) << 16 | GPACK_RGBA5551(0,0xc0,0,1));
    gDPFillRectangle(glistp++, 0, SCREEN_HT - 8, (u32)((SCREEN_WD) * percentage), SCREEN_HT);
  }
#endif


  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  nuDebConClear(0);
  nuDebConTextPos(0,4,4);
  sprintf(conbuf,"title screen");
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0,4,5);
  sprintf(conbuf,"        t: %2.2f/%2.2f", spotTimePassed, spots[spotIndex].duration);
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0,4,6);
  sprintf(conbuf,"spotIndex: %03d", spotIndex);
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0,4,7);
  sprintf(conbuf,"   time: %2.2f", timePassed);
  nuDebConCPuts(0, conbuf);
    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateTitleScreen() {
  nuContDataGetEx(contdata,0);

  timePassed += deltaTimeSeconds;

  if (spotIndex < (NUMBER_OF_SPOTS)) {
    spotTimePassed += deltaTimeSeconds;

    if (spotTimePassed > spots[spotIndex].duration) {
      spotTimePassed = 0.f;
      spotIndex++;
    }
  }
  

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
