
#include "cutscene.h"

#include <nusys.h>
#include <assert.h>

#include "backgroundbuffers.h"
#include "main.h"
#include "graphic.h"
#include "nustdfuncs.h"
#include "segmentinfo.h"
#include "dialogue.h"
#include "cutscenes/cutscenelookup.h"
#include "cutscene_backgrounds/backgroundlookup.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

static CutsceneInfo infoForOurCutscene;

const char* cutsceneToLoad;

static float cutsceneTime;

#define FADE_TIME_BETWEEN_BACKGROUNDS 1.21616f
u8 backgroundIndex;
static u8 internalBackgroundIndex;
static u8 isFading;
static float backgroundFadeTime;

#define FADE_IN_TIME 1.24f
#define FADE_OUT_TIME 1.5f
#define DONE_TIME 0.56f

#define CUTSCENE_FADING_IN 0
#define CUTSCENE_PLAYING 1
#define CUTSCENE_FADING_OUT 2
#define CUTSCENE_DONE 3
static u8 cutsceneState;

void initCutscene() {
  cutsceneTime = 0.f;
  cutsceneState = CUTSCENE_FADING_IN;

  backgroundIndex = 0;
  internalBackgroundIndex = 0;
  isFading = 0;
  backgroundFadeTime = 0.f;

  struct cutsceneMappingData* cutsceneOffsetInfo = getCutsceneOffset(cutsceneToLoad, _nstrlen(cutsceneToLoad));
  assert(cutsceneOffsetInfo != 0x0);
  nuPiReadRom((u32)(_cutscenebuffersSegmentRomStart + cutsceneOffsetInfo->offset), &infoForOurCutscene, sizeof(CutsceneInfo));

  struct backgroundMappingData* bg1 = getBackgroundTextureOffset((const char*)(infoForOurCutscene.imageKey1), _nstrlen((const char*)(infoForOurCutscene.imageKey1)));
  if (bg1 != NULL) {
    nuPiReadRom((u32)(_packedbackgroundsSegmentRomStart + bg1->offset), backgroundBuffers[0], 320 * 240 * 2);
  } else {
    bzero(backgroundBuffers[0], 320 * 240 * 2);
  }

  struct backgroundMappingData* bg2 = getBackgroundTextureOffset((const char*)(infoForOurCutscene.imageKey2), _nstrlen((const char*)(infoForOurCutscene.imageKey2)));
  if (bg2 != NULL) {
    nuPiReadRom((u32)(_packedbackgroundsSegmentRomStart + bg2->offset), backgroundBuffers[1], 320 * 240 * 2);
  } else {
    bzero(backgroundBuffers[1], 320 * 240 * 2);
  }

  struct backgroundMappingData* bg3 = getBackgroundTextureOffset((const char*)(infoForOurCutscene.imageKey3), _nstrlen((const char*)(infoForOurCutscene.imageKey3)));
  if (bg3 != NULL) {
    nuPiReadRom((u32)(_packedbackgroundsSegmentRomStart + bg3->offset), backgroundBuffers[2], 320 * 240 * 2);
  } else {
    bzero(backgroundBuffers[2], 320 * 240 * 2);
  }
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

  if (cutsceneState == CUTSCENE_FADING_IN) {
    float t = (cutsceneTime / FADE_IN_TIME);
    int tVal = t * 255;
    gDPSetPrimColor(glistp++, 0, 0, tVal, tVal, tVal, (tVal));

    gDPSetCombineMode(glistp++,G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
  } else if (cutsceneState == CUTSCENE_FADING_OUT) {
    float t = 1.f - (cutsceneTime / FADE_OUT_TIME);
    int tVal = t * 255;
    gDPSetPrimColor(glistp++, 0, 0, tVal, tVal, tVal, tVal);
    gDPSetCombineMode(glistp++,G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
  } else {
    gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  }


  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);

  if (cutsceneState != CUTSCENE_DONE) {
    for (int i = 0; i < (240 / 6); i++) {
      gDPPipeSync(glistp++);
      gDPLoadTextureTile(glistp++, backgroundBuffers[internalBackgroundIndex], G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, 240, 0, (i * 6), 320 - 1, ((i + 1) * 6) - 1, 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD );
      gSPTextureRectangle(glistp++, 0 << 2, (0 + (i * 6)) << 2, (0 + 320) << 2, (0 + ((i + 1) * 6)) << 2, 0, 0 << 5, (i * 6) << 5, 1 << 10, 1 << 10);
    }

    if ( internalBackgroundIndex != backgroundIndex) {
      for (int i = 0; i < (int)((backgroundFadeTime / FADE_TIME_BETWEEN_BACKGROUNDS) * (240 / 2)); i++) {
        gDPPipeSync(glistp++);
        gDPLoadTextureTile(glistp++, backgroundBuffers[backgroundIndex], G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, 240, 0, (i * 2), 320 - 1, ((i + 1) * 2) - 1, 0, G_TX_WRAP, G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD );
        gSPScisTextureRectangle(glistp++, 0 << 2, (0 + (i * 2)) << 2, (0 + 320) << 2, (0 + ((i + 1) * 2)) << 2, 0, 0 << 5, (i * 2) << 5, 1 << 10, 1 << 10);
      }

    }

  }

  renderDialogueToDisplayList();

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_NOSWAPBUFFER);

  nuDebConClear(0);
  nuDebConTextPos(0, 2, 22);
  sprintf(conbuf,"cutscene");
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0, 2, 24);
  sprintf(conbuf," audio heap: 0x%08x", NU_AU_HEAP_ADDR);
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0, 2, 25);
  sprintf(conbuf,"our bss end: 0x%08x", _codeSegmentBssEnd);
  nuDebConCPuts(0, conbuf);

  nuDebConTextPos(0, 2, 26);
  sprintf(conbuf,"  remaining: 0d%08u", (u32)(NU_AU_HEAP_ADDR) - (u32)(_codeSegmentBssEnd));
  nuDebConCPuts(0, conbuf);

    
  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateFadingIn() {
  cutsceneTime += deltaTimeSeconds;

  if (cutsceneTime > FADE_IN_TIME) {
    cutsceneState = CUTSCENE_PLAYING;

    startDialogue((const char*)(infoForOurCutscene.dialogue));
  }
}

void updatePlayingDialogue() {

  if ((!isFading) && (backgroundIndex != internalBackgroundIndex)) {
    isFading = 1;
    backgroundFadeTime = 0.f;
  } else if (isFading) {
    backgroundFadeTime += deltaTimeSeconds;
    if (backgroundFadeTime > FADE_TIME_BETWEEN_BACKGROUNDS) {
      isFading = 0;
      internalBackgroundIndex = backgroundIndex;
      backgroundFadeTime = 0.f;
    }
  }

  if (dialogueState == DIALOGUE_STATE_SHOWING) {
    return;
  }

  cutsceneTime = 0.f;
  cutsceneState = CUTSCENE_FADING_OUT;
}

void updateFadingOut() {
  cutsceneTime += deltaTimeSeconds;

  if (cutsceneTime > FADE_OUT_TIME) {

    cutsceneTime = 0.f;
    cutsceneState = CUTSCENE_DONE;
  }
}

void updateCutsceneDone() {
  cutsceneTime += deltaTimeSeconds;

  if (cutsceneTime > DONE_TIME) {
    nextStage = &levelSelectStage;
    changeScreensFlag = 1;

  }
}

void updateCutscene() {
  nuContDataGetEx(contdata,0);

  switch (cutsceneState) {
    case CUTSCENE_FADING_IN:
      updateFadingIn();
      break;
    case CUTSCENE_PLAYING:
      updatePlayingDialogue();
      break;
    case CUTSCENE_FADING_OUT:
      updateFadingOut();
      break;
    case CUTSCENE_DONE:
      updateCutsceneDone();
      break;
  }

  // if (contdata[0].trigger & START_BUTTON) {
  //   nextStage = &levelSelectStage;
  //   changeScreensFlag = 1;
  // }
}
