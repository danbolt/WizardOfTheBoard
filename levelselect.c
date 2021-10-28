
#include "levelselect.h"

#include <nusys.h>

#include "displaytext.h"
#include "main.h"
#include "gamemath.h"
#include "graphic.h"
#include "nustdfuncs.h"
#include "segmentinfo.h"
#include "stagekeys.h"
#include "audio/sfx/sfx.h"
#include "audio/bgm/sequence/tracknumbers.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

static u8 backgroundTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));
static u8 iconsTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

static u32 currentlySelectedLevel;

static float timePassed;

static u8 inTheOptionsPanel;
static u8 optionsIndex;
static u8 sfxIndex;
static u8 bgmIndex;
static float horizontalSwipe;

#define OPTIONS_0_FOV 0
#define OPTIONS_1_FLASHING 1
#define OPTIONS_2_SFX_TEST 2
#define OPTIONS_3_BGM_TEST 3
#define OPTIONS_COUNT 4

static u8 downPressed;
static u8 upPressed;
static u8 rightPressed;
static u8 leftPressed;
static u8 stickInDeadzone;

static float selectedLevelLerpValue;
static float slideOutLerpValue;

static char textBuffer[32];

#define NOT_TRANSITIONING 0
#define TRANSITIONING_IN 1
#define TRANSITIONING_OUT 2
#define TRANSITION_DURATION 0.8f
static u8 transitioningState;
static float transitionTime;

void initLevelSelect() {
  currentlySelectedLevel = currentLevel % NUMBER_OF_LEVELS;
  selectedLevelLerpValue = 0.f;
  slideOutLerpValue = 0.f;
  timePassed = 0.f;

  optionsIndex = 0;
  inTheOptionsPanel = 0;
  horizontalSwipe = 0.f;

  sfxIndex = 0;
  bgmIndex = 0;

  downPressed = 0;
  upPressed = 0;
  rightPressed = 0;
  leftPressed = 0;
  stickInDeadzone = 0;

  textBuffer[0] = '\0';

  transitioningState = TRANSITIONING_IN;
  transitionTime = 0.f;

  nuPiReadRom((u32)_level_select_backgroundSegmentRomStart, backgroundTexture, TMEM_SIZE_BYTES);
  nuPiReadRom((u32)_level_select_iconsSegmentRomStart, iconsTexture, TMEM_SIZE_BYTES);
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

  const int swipeOffset = (int)horizontalSwipe;

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPSetTextureFilter(glistp++, G_TF_POINT);
  gDPSetCombineMode(glistp++, G_CC_MODULATEIDECALA_PRIM, G_CC_MODULATEIDECALA_PRIM);
  gDPSetRenderMode(glistp++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++, G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);

  gDPPipeSync(glistp++);
  gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(backgroundTexture), G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
  gDPSetPrimColor(glistp++, 0, 0, 0x00, 0x33, 0x61, 0xff);
  gSPTextureRectangle(glistp++, (0) << 2, (0) << 2, (SCREEN_WD) << 2, (SCREEN_HT) << 2, 0, ((u32)(timePassed * 1.f * 64.f)) << 5, ((u32)(timePassed * 0.7f * 64.f)) << 5, 1 << 10, 1 << 10);
  
  // Options pane
  gDPSetPrimColor(glistp++, 0, 0, 0x00, 0x23, 0x41, 0xff);
  gSPScisTextureRectangle(glistp++, (-128 + ((int)swipeOffset)) << 2, (0) << 2, (-128 + ((int)swipeOffset) + 164) << 2, (SCREEN_HT) << 2, 0, ((u32)(timePassed * 1.f * 64.f)) << 5, ((u32)(timePassed * 0.7f * 64.f)) << 5, 1 << 10, 1 << 10);

  gDPPipeSync(glistp++);
  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(displayTextTexture), G_IM_FMT_IA, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  renderDisplayText(swipeOffset - 128 + TITLE_SAFE_HORIZONTAL + 16, TITLE_SAFE_VERTICAL + 8, "OPTIONS");


  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  if (inTheOptionsPanel && (optionsIndex == OPTIONS_0_FOV)) {
    gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
  }
  sprintf(textBuffer, "FOV %02d", (int)ingameFOV);
  renderDisplayText(swipeOffset - 128 + TITLE_SAFE_HORIZONTAL + 8, TITLE_SAFE_VERTICAL + 8 + 32, textBuffer);


  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  if (inTheOptionsPanel && (optionsIndex == OPTIONS_1_FLASHING)) {
    gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
  }
  sprintf(textBuffer, "BULLETS\n   %s", flashingProjectiles ? "FLASHING" : "SHADED");
  renderDisplayText(swipeOffset - 128 + TITLE_SAFE_HORIZONTAL , TITLE_SAFE_VERTICAL + 8 + (32 * 2), textBuffer);

  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  if (inTheOptionsPanel && (optionsIndex == OPTIONS_2_SFX_TEST)) {
    gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
  }
  sprintf(textBuffer, "SFX TEST\n    %02d", sfxIndex);
  renderDisplayText(swipeOffset - 128 + TITLE_SAFE_HORIZONTAL + 8, TITLE_SAFE_VERTICAL + 8 + (32 * 3) + 16, textBuffer);

  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  if (inTheOptionsPanel && (optionsIndex == OPTIONS_3_BGM_TEST)) {
    gDPSetPrimColor(glistp++, 0, 0, N64_C_BUTTONS_RED, N64_C_BUTTONS_GREEN, N64_C_BUTTONS_BLUE, 0xff);
  }
  sprintf(textBuffer, "BGM TEST\n    %02d", bgmIndex);
  renderDisplayText(swipeOffset - 128 + TITLE_SAFE_HORIZONTAL + 8, TITLE_SAFE_VERTICAL + 8 + (32 * 4) + 32, textBuffer);


  // Level select
  gDPPipeSync(glistp++);
  gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(iconsTexture), G_IM_FMT_IA, G_IM_SIZ_8b, 256, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
  gDPSetPrimColor(glistp++, 0, 0, 0x99 >> 1, 0x42 >> 1, 0x8C >> 1, 0xff);
  for (int i = 0; i < NUMBER_OF_LEVELS; i++) {
    const s32 offset = (int)(((float)i - (selectedLevelLerpValue)) * -16.f);
    gSPScisTextureRectangle(glistp++, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 64) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 16 + offset - 32) << 2, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL + offset - 32) << 2, 0, ((i % 3) * 64) << 5, 0 << 5, 1 << 10, 1 << 10);
  }
  gDPSetPrimColor(glistp++, 0, 0, 0x99, 0x42, 0x8C, 0xff);
  for (int i = 0; i < NUMBER_OF_LEVELS; i++) {
    int xOffset = (i == currentlySelectedLevel) ? (int)(slideOutLerpValue) : 0;
    const s32 offset = (int)(((float)i - (selectedLevelLerpValue)) * -16.f);
    gSPScisTextureRectangle(glistp++, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 64 + xOffset) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 16 + offset - 32) << 2, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 + xOffset) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL + offset - 32) << 2, 0, ((i % 3) * 64) << 5, 0 << 5, 1 << 10, 1 << 10);
  }
  gSPScisTextureRectangle(glistp++, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 64) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 16 + ((int)((NUMBER_OF_LEVELS - selectedLevelLerpValue) * -16)) - 32) << 2, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 48) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL + ((int)((NUMBER_OF_LEVELS - selectedLevelLerpValue) * -16)) - 32) << 2, 0, 192 << 5, 0 << 5, 1 << 10, 1 << 10);
  gSPScisTextureRectangle(glistp++, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 64 + 48) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 16 + ((int)((NUMBER_OF_LEVELS - selectedLevelLerpValue) * -16)) - 32) << 2, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 48 + 48) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL + ((int)((NUMBER_OF_LEVELS - selectedLevelLerpValue) * -16)) - 32) << 2, 0, 224 << 5, 0 << 5, 1 << 10, 1 << 10);

  const int cursorOffset = (int)(sinf(timePassed * 8.1616f) * 4.f);
  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  gSPTextureRectangle(glistp++, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 64 - 8 - 32 + cursorOffset) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 48 + 4) << 2, (swipeOffset + SCREEN_WD - TITLE_SAFE_HORIZONTAL - 64 - 64 - 8 - 16 + cursorOffset) << 2, (SCREEN_HT - TITLE_SAFE_VERTICAL - 32 + 4) << 2, 0, 208 << 5, 0 << 5, 1 << 10, 1 << 10);

  gDPPipeSync(glistp++);
  gDPSetPrimColor(glistp++, 0, 0, 0xff, 0xff, 0xff, 0xff);
  gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(displayTextTexture), G_IM_FMT_IA, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  sprintf(textBuffer, "FLOOR %01d", (currentlySelectedLevel + 1));
  renderDisplayText(swipeOffset + TITLE_SAFE_HORIZONTAL + 48, 160, textBuffer);

  if (transitioningState != NOT_TRANSITIONING) {
    gDPPipeSync(glistp++);
    gDPSetCycleType(glistp++, G_CYC_FILL);
    gDPSetFillColor(glistp++, GPACK_RGBA5551(0,0,0,1) << 16 | GPACK_RGBA5551(0,0,0,1));

    float t = (transitionTime / TRANSITION_DURATION);
    if (transitioningState == TRANSITIONING_IN) {
      t = 1.f - t;
    }
    t = cubic(t);

    for (int i = 0; i < (SCREEN_HT / 24); i++) {
      if (i % 2 == 0) {
        gDPFillRectangle(glistp++, 0, i * 24, (int)(SCREEN_WD * t), (i + 1) * 24);
      } else {  
        gDPFillRectangle(glistp++, (int)(SCREEN_WD * (1.f - t)), i * 24, SCREEN_WD, (i + 1) * 24);
      }
    }
  }

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateLevelSelectTransition() {
  if (transitioningState == NOT_TRANSITIONING) {
    return;
  }

  transitionTime += deltaTimeSeconds;

  if (transitionTime > TRANSITION_DURATION) {
    if (transitioningState == TRANSITIONING_IN) {
      transitioningState = NOT_TRANSITIONING;
    } else if (transitioningState == TRANSITIONING_OUT) {
      changeScreensFlag = 1;
    }
  }
}

void updateInput() {
  if (transitioningState != NOT_TRANSITIONING) {
    return;
  }

  selectedLevelLerpValue = lerp(selectedLevelLerpValue, currentlySelectedLevel, 0.19f);
  slideOutLerpValue = lerp(slideOutLerpValue, -32.f, 0.19f);

  // I know this is kind of "backwards" but my hubris/laziness prevents me from addressing it 
  if(contdata[0].trigger & U_JPAD) {
    downPressed = 1;
  } else if(contdata[0].trigger & D_JPAD) {
    upPressed = 1;
  } else if(contdata[0].trigger & L_JPAD) {
    leftPressed = 1;
  } else if(contdata[0].trigger & R_JPAD) {
    rightPressed = 1;
  } else {
    upPressed = 0;
    downPressed = 0;
    leftPressed = 0;
    rightPressed = 0;
  }

  if (!stickInDeadzone && (contdata[0].stick_y > -7) && (contdata[0].stick_y < 7) && (contdata[0].stick_x > -7) && (contdata[0].stick_x < 7)) {
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

    if (contdata[0].stick_x < -7) {
      leftPressed = 1;
      stickInDeadzone = 0;
    } else if (contdata[0].stick_x > 7) {
      rightPressed = 1;
      stickInDeadzone = 0;
    }

  }


  if (upPressed) {
    if (inTheOptionsPanel) {
      optionsIndex = (optionsIndex + 1) % OPTIONS_COUNT;

      if (optionsIndex == OPTIONS_3_BGM_TEST) {
        nuAuSeqPlayerStop(0);
      }
    } else {
      currentlySelectedLevel = (currentlySelectedLevel - 1 + NUMBER_OF_LEVELS) % NUMBER_OF_LEVELS;
      slideOutLerpValue = 0.f;
    }

    upPressed = 0;
    nuAuSndPlayerPlay(SFX_02_NOBODY_BIP);
  }
  if (downPressed) {
    if (inTheOptionsPanel) {
      optionsIndex = (optionsIndex - 1 + OPTIONS_COUNT) % OPTIONS_COUNT;

      if (optionsIndex == OPTIONS_3_BGM_TEST) {
        nuAuSeqPlayerStop(0);
      }
    } else {
      currentlySelectedLevel = (currentlySelectedLevel + 1) % NUMBER_OF_LEVELS;
      slideOutLerpValue = 0.f;
    }

    downPressed = 0;
    nuAuSndPlayerPlay(SFX_02_NOBODY_BIP);
  }

  if (leftPressed && (!inTheOptionsPanel)) {
    inTheOptionsPanel = 1;
    leftPressed = 0;

    nuAuSndPlayerPlay(SFX_02_NOBODY_BIP);
  }
  if (rightPressed && inTheOptionsPanel) {
    inTheOptionsPanel = 0;
    rightPressed = 0;

    nuAuSndPlayerPlay(SFX_02_NOBODY_BIP);
  }

  if (inTheOptionsPanel) {
    if (contdata[0].trigger & A_BUTTON) {
      if (optionsIndex == OPTIONS_0_FOV) {
        if (fabsf(ingameFOV - FOV_60) < 1.f) {
          ingameFOV = FOV_90;
        } else {
          ingameFOV = FOV_60;
        }
      } else if (optionsIndex == OPTIONS_1_FLASHING) {
        flashingProjectiles = !flashingProjectiles;
      } else if (optionsIndex == OPTIONS_2_SFX_TEST) {
        nuAuSndPlayerPlay(sfxIndex);
      } else if (optionsIndex == OPTIONS_3_BGM_TEST) {
        if (nuAuSeqPlayerGetState(0) == AL_STOPPED) {
          nuAuSeqPlayerStop(0);
          nuAuSeqPlayerSetNo(0, bgmIndex);
          nuAuSeqPlayerPlay(0);
        } else if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
          nuAuSeqPlayerStop(0);
        }
      }
    }

    if (contdata[0].trigger & R_CBUTTONS) {
      if (optionsIndex == OPTIONS_2_SFX_TEST) {
        sfxIndex = (sfxIndex + 1) % SFX_COUNT;
      } else if (optionsIndex == OPTIONS_3_BGM_TEST) {
        bgmIndex = (bgmIndex + 1) % TRACK_COUNT;

        if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
          nuAuSeqPlayerStop(0);
        }
      }
    } else if(contdata[0].trigger & L_CBUTTONS) {
      if (optionsIndex == OPTIONS_2_SFX_TEST) {
        sfxIndex = (sfxIndex - 1 + SFX_COUNT) % SFX_COUNT;
      } else if (optionsIndex == OPTIONS_3_BGM_TEST) {
        bgmIndex = (bgmIndex - 1 + TRACK_COUNT) % TRACK_COUNT;

        if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
          nuAuSeqPlayerStop(0);
        }
      }
    }
  } else {
    if (contdata[0].trigger & A_BUTTON) {
      currentLevel = currentlySelectedLevel;
      nextStage = &gameplayStage;
      transitioningState = TRANSITIONING_OUT;
      transitionTime = 0.f;
      nuAuSndPlayerPlay(SFX_11_MENU_CONFIRM);

      if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
        nuAuSeqPlayerFadeOut(0, 25);
      }
    } else if (contdata[0].trigger & B_BUTTON) {
      nextStage = &titleScreenStage;
      transitioningState = TRANSITIONING_OUT;
      transitionTime = 0.f;
      nuAuSndPlayerPlay(SFX_12_MENU_BACK);

      if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
        nuAuSeqPlayerFadeOut(0, 25);
      }
    }
  }
}

void updateLevelSelect() {
  nuContDataGetEx(contdata,0);

  timePassed += deltaTimeSeconds;

  horizontalSwipe = lerp(horizontalSwipe, inTheOptionsPanel ? 128 : 20, 0.13f);

  updateLevelSelectTransition();
  updateInput();
}
