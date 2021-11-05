#include "credits.h"

#include <nusys.h>

#include "audio/bgm/sequence/tracknumbers.h"
#include "gameaudio.h"
#include "displaytext.h"
#include "graphic.h"
#include "main.h"
#include "sixtwelve.h"
#include "sixtwelve_helpers.h"

#define MAX_NAMES_PER_CREDIT_ITEM 8

#define CREDIT_FADE_TIME 0.5f

typedef struct {
  const char* title;

  u32 numberOfItems;
  const char* names[MAX_NAMES_PER_CREDIT_ITEM];
  float extraLeadIn;
} CreditsItem;

static CreditsItem generalDev = {
  "DEVELOPED BY",

  1,
  {
  "Daniel Savage",
  },
  1.f
};

static CreditsItem voiceActing = {
  "VOICES BY",

  2,
  {
    "Clayton Savage",
    "Daniel Savage"
  },
  1.f
};

// This one's hacky but I like it
static CreditsItem unfLoader = {
  "UNFLOADER\n      AND\nSAUSAGE64",

  3,
  {
  "",
  "",
  "Buu342",
  },
  0.f
};

static CreditsItem palTesting = {
  "PAL TESTING",

  2,
  {
  "kivan117",
  "gravatos",
  },
  0.f
};

static CreditsItem specialThanks = {
  "SPECIAL THANKS",

  4,
  {
  "Natasha Miner",
  "Mom, Dad, Alyssa, and Roro <3",
  "kivan117",
  "Hazematman",
  },
  0.f
};

static CreditsItem* itemsToShow[] = {
  &generalDev,
  &voiceActing,
  &unfLoader,
  &palTesting,
  &specialThanks,
  NULL
};

static int currentCreditIndex;
static u32 fadedOutMusic;

static float creditsTime;

int creditDuration(const CreditsItem* item) {
  return item->extraLeadIn + MAX(3.6f, (2.4f + (item->numberOfItems * 0.4f)));
}

void initCredits() {
  creditsTime = 0.f;

  currentCreditIndex = 0;
  fadedOutMusic = 0;

  playMusic(TRACK_08_OVERTURE_ENDING);
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


  if (itemsToShow[currentCreditIndex] && (creditsTime - itemsToShow[currentCreditIndex]->extraLeadIn) < CREDIT_FADE_TIME) {
    float t = ((creditsTime - itemsToShow[currentCreditIndex]->extraLeadIn) / CREDIT_FADE_TIME);
    int tVal = t * 255;
    gDPSetPrimColor(glistp++, 0, 0, tVal, tVal, tVal, 255);
    gDPSetCombineMode(glistp++,G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM);

  } else if (itemsToShow[currentCreditIndex] && ((creditsTime ) > (creditDuration(itemsToShow[currentCreditIndex]) - CREDIT_FADE_TIME))) {
    float t = 1.f - (((creditsTime - itemsToShow[currentCreditIndex]->extraLeadIn) - (creditDuration(itemsToShow[currentCreditIndex]) - CREDIT_FADE_TIME)) / CREDIT_FADE_TIME);
    int tVal = t * 255;
    gDPSetPrimColor(glistp++, 0, 0, tVal, tVal, tVal, 255);
    gDPSetCombineMode(glistp++,G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM);

  } else {
    gDPSetCombineMode(glistp++,G_CC_DECALRGBA, G_CC_DECALRGBA);
  }


  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
  gSPTexture(glistp++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);
  gSPClipRatio(glistp++, FRUSTRATIO_2);
  
  

  if ((itemsToShow[currentCreditIndex] != NULL) && (creditsTime > itemsToShow[currentCreditIndex]->extraLeadIn)) {

    const int namesVerticalSpot = (SCREEN_HT - (MIN(MAX_NAMES_PER_CREDIT_ITEM ,itemsToShow[currentCreditIndex]->numberOfItems) * (SIXTWELVE_LINE_HEIGHT + 2))) / 2 + 16;

    const int titleWidth = measureDisplayText(itemsToShow[currentCreditIndex]->title);
    const int titleVerticalSpot = namesVerticalSpot - 32;
    const int titleHorizontalSpot = (SCREEN_WD - titleWidth) / 2;

    gDPPipeSync(glistp++);
    gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(displayTextTexture), G_IM_FMT_IA, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    renderDisplayText(titleHorizontalSpot, titleVerticalSpot, itemsToShow[currentCreditIndex]->title);

    gDPPipeSync(glistp++);
    gDPLoadTextureBlock_4b(glistp++, sixtwelve_tex, G_IM_FMT_IA, SIXTWELVE_TEXTURE_WIDTH, SIXTWELVE_TEXTURE_HEIGHT, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    for (int i = 0; i < MIN(MAX_NAMES_PER_CREDIT_ITEM ,itemsToShow[currentCreditIndex]->numberOfItems); i++) {

      s32 lineWidth = (s32)sixtwelve_calculate_string_crass_width(itemsToShow[currentCreditIndex]->names[i]);
      int xSpot = (SCREEN_WD - (lineWidth / 2)) / 2;
      writeString(xSpot, namesVerticalSpot + (i * (SIXTWELVE_LINE_HEIGHT + 2)), itemsToShow[currentCreditIndex]->names[i]);
    }
  }

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0], (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx), NU_GFX_UCODE_F3DLP_REJ , NU_SC_SWAPBUFFER);

  gfx_gtask_no = (gfx_gtask_no + 1) % BUFFER_COUNT;
}

void updateCredits() {
  creditsTime += deltaTimeSeconds;

  if (itemsToShow[currentCreditIndex] && (creditsTime > creditDuration(itemsToShow[currentCreditIndex]))) {
    currentCreditIndex++;
    creditsTime = 0.f;
  } else if ((itemsToShow[currentCreditIndex] == NULL) && (creditsTime > 5.f)) {
    nextStage = &titleScreenStage;
    changeScreensFlag = 1;
  }

  if ((itemsToShow[currentCreditIndex] == NULL) && (creditsTime > 3.f) && (!fadedOutMusic)) {
    fadedOutMusic = 1;


    fadeOutMusic();
  }
}
