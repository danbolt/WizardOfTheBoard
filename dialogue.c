
#include "dialogue.h"

#include "cutscene.h"
#include "graphic.h"
#include "cast_sprites/castlookup.h"
#include "dialogue/dialoguelookup.h"
#include "main.h"
#include "nustdfuncs.h"
#include "segmentinfo.h"
#include "sixtwelve.h"
#include "sixtwelve_helpers.h"

u32 dialogueState;

static int bipIndex;
static float bipTimePassed;

static int dialogueBoxY;

#define BIP_TIME_SECONDS 0.03f

#define STRUCT_FLAG_SHOW_BG_1 1
#define STRUCT_FLAG_SHOW_BG_2 2
#define STRUCT_FLAG_SHOW_BG_3 3

typedef union {
  DialogueItem item;
  u8 aligner[320]; // padding to ensure DMA alignment
} DMAAlignedDialogueItem;

#define NUMBER_OF_DIALOGUE_ITEM_BUFFERS 3
static DMAAlignedDialogueItem dialogueItemTripleBuffer[NUMBER_OF_DIALOGUE_ITEM_BUFFERS] __attribute__((aligned(8)));
static u8 portraitBuffer0[TMEM_SIZE_BYTES * 2] __attribute__((aligned(8)));
static u8 portraitBuffer1[TMEM_SIZE_BYTES * 2] __attribute__((aligned(8)));
static u8 portraitBuffer2[TMEM_SIZE_BYTES * 2] __attribute__((aligned(8)));
static u8* portraitBuffers[NUMBER_OF_DIALOGUE_ITEM_BUFFERS] = { portraitBuffer0, portraitBuffer1, portraitBuffer2 };
static u8* currentPortrait;
static int nextDialogueItemIndex;
static DialogueItem* currentDialogueItem;

static u8 dialogueBackingBuffer[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

// TODO: add a "simple string" drawing function as well

void drawString(int x, int y, const unsigned char* str, int maxWordWrapWidth) {
  int stringIndex = 0;
  int xLetterSpot = x;
  int yLetterSpot = y;
  int verticalLineCount = 1;
  while (str[stringIndex] != '\0') {
    if (stringIndex >= bipIndex) {
      break;
    }

    const unsigned char character = str[stringIndex];

    // If the next word is too big for the word wrap width, go to a new line
    if (maxWordWrapWidth > 0) {
      unsigned int remainingWidth = sixtwelve_calculate_string_width(&(str[stringIndex]));
      if (((xLetterSpot - x) + remainingWidth) > maxWordWrapWidth) {
        xLetterSpot = x;
        yLetterSpot += SIXTWELVE_LINE_HEIGHT;
      }
    }

    if (character == '\n') {
      xLetterSpot = x;
      yLetterSpot += SIXTWELVE_LINE_HEIGHT;
      stringIndex++;
      continue;
    }


    const sixtwelve_character_info* characterInfo = sixtwelve_get_character_info(character);

    const int xLoc = xLetterSpot + characterInfo->x_offset;
    const int yLoc = yLetterSpot + characterInfo->y_offset;

    gSPScisTextureRectangle(glistp++, (xLoc) << 2, (yLoc) << 2, (xLoc + characterInfo->width) << 2, (yLoc + characterInfo->height) << 2, 0, (characterInfo->x) << 5, (characterInfo->y) << 5, 1 << 10, 1 << 10);

    xLetterSpot += characterInfo->x_advance;
    stringIndex++;
  }
}

void initalizeDialogue() {
  dialogueBoxY = TITLE_SAFE_VERTICAL + 16;

  dialogueState = DIALOGUE_STATE_OFF;
  bipIndex = 0;
  bipTimePassed = 0.f;
  nextDialogueItemIndex = 0;
  currentDialogueItem = NULL;
  currentPortrait = NULL;

  nuPiReadRom((u32)(_dialogue_backingSegmentRomStart), (void*)(dialogueBackingBuffer), TMEM_SIZE_BYTES);
}

void startDialogueItem(u32 offset) {
  bipIndex = 0;
  bipTimePassed = 0.f;

  DialogueItem* nextDialogueItem = &(dialogueItemTripleBuffer[nextDialogueItemIndex].item);

  nuPiReadRom((u32)(_dialogue_dataSegmentRomStart + offset), (void*)(nextDialogueItem), sizeof(DialogueItem));

  currentDialogueItem = nextDialogueItem;

  struct castMappingData * castSpriteOffset = getCastTextureOffset(nextDialogueItem->speaker, _nstrlen(nextDialogueItem->speaker));
  if (castSpriteOffset) {

    nuPiReadRom((u32)(_cast_sprite_dataSegmentRomStart + castSpriteOffset->offset), portraitBuffers[nextDialogueItemIndex], TMEM_SIZE_BYTES * 2);
    currentPortrait = portraitBuffers[nextDialogueItemIndex];
  } else {
    currentPortrait = NULL;
  }

  if (nextDialogueItem->flags[0] > 0) {
    if (nextDialogueItem->flags[0] == STRUCT_FLAG_SHOW_BG_1) {
      backgroundIndex = 0;
    } else if (nextDialogueItem->flags[0] == STRUCT_FLAG_SHOW_BG_2) {
      backgroundIndex = 1;
    } else if (nextDialogueItem->flags[0] == STRUCT_FLAG_SHOW_BG_3) {
      backgroundIndex = 2;
    }
  }

  nextDialogueItemIndex = (nextDialogueItemIndex + 1) % NUMBER_OF_DIALOGUE_ITEM_BUFFERS;

  dialogueState = DIALOGUE_STATE_SHOWING;
}

u32 lookupOffsetForDialogueKey(const char* key, u32* result) {
  if (result == NULL) {
    return 0;
  }

  struct dialogueMappingData * mapping = getDialogueDataOffset(key, _nstrlen(key));
  if (mapping == NULL) {
    return 0;
  }

  *result = mapping->offset;
  return 1;
}

void startDialogue(const char* key) {
  if (dialogueState == DIALOGUE_STATE_SHOWING) {
    return;
  }

  u32 dialogueLookup = 0x0;
  if (!(lookupOffsetForDialogueKey(key, &dialogueLookup))) {
    return;
  }

  startDialogueItem(dialogueLookup);

  
}

void updateDialogue() {
  if (dialogueState == DIALOGUE_STATE_OFF) {
    return;
  }


  if (currentDialogueItem->text[bipIndex] != '\0') {
    bipTimePassed += deltaTimeSeconds;
    if (bipTimePassed > BIP_TIME_SECONDS) {
      bipTimePassed = 0.f;
      bipIndex++;

    }

    // If the player presses the confirm button, skip ahead to the end of the dialogue.
    if (contdata[0].trigger & A_BUTTON) {
      while (currentDialogueItem->text[bipIndex] != '\0') {
        bipIndex++;
      }
    }
  } else {
    if (contdata[0].trigger & A_BUTTON) {
      
      if (currentDialogueItem->nextAddress != 0x0) {
        startDialogueItem((u32)(currentDialogueItem->nextAddress));
      } else {
        dialogueState = DIALOGUE_STATE_OFF;
      }
    }
  }
}

void renderDialogueToDisplayList() {
  if (dialogueState == DIALOGUE_STATE_OFF) {
    return;
  }
  // gDPPipeSync(glistp++);
  // gDPSetCycleType(glistp++, G_CYC_FILL);
  // gDPSetFillColor(glistp++, GPACK_RGBA5551(0,0,0,1) << 16 | GPACK_RGBA5551(0,0,0,1));
  // gDPFillRectangle(glistp++, TITLE_SAFE_HORIZONTAL, dialogueBoxY - 4, (SCREEN_WD - TITLE_SAFE_HORIZONTAL), dialogueBoxY + (64) + 4);
  // gDPPipeSync(glistp++);
  // gDPSetCycleType(glistp++, G_CYC_1CYCLE);

  gDPPipeSync(glistp++);
  gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(dialogueBackingBuffer), G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
  gSPTextureRectangle(glistp++, (TITLE_SAFE_HORIZONTAL + 10) << 2, (dialogueBoxY - 4) << 2, (TITLE_SAFE_HORIZONTAL + 256 + 10) << 2, (dialogueBoxY + 64 + 4) << 2, 0, 0 << 5, 0 << 5, (0 << 10) | (2 << 7), (0 << 10) | (7 << 7));


  gDPPipeSync(glistp++);
  gDPLoadTextureBlock_4b(glistp++, sixtwelve_tex, G_IM_FMT_IA, SIXTWELVE_TEXTURE_WIDTH, SIXTWELVE_TEXTURE_HEIGHT, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  if (currentPortrait != NULL) {
    drawString(TITLE_SAFE_HORIZONTAL + 32 + 10 + 10, dialogueBoxY, currentDialogueItem->text, 256 - 32 - 6);


    gDPLoadTextureBlock(glistp++, OS_K0_TO_PHYSICAL(currentPortrait + ((((bipIndex >> 1) % 2 == 0) && (currentDialogueItem->text[bipIndex] != '\0')) ? 0 : TMEM_SIZE_BYTES)), G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetCombineMode(glistp++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gSPTextureRectangle(glistp++, (TITLE_SAFE_HORIZONTAL + 6 + 10) << 2, dialogueBoxY << 2, ((TITLE_SAFE_HORIZONTAL + 4 + 10) + 32) << 2, (dialogueBoxY + 64) << 2, 0, 0 << 5, 0 << 5, 1 << 10, 1 << 10);
  } else {
    drawString(TITLE_SAFE_HORIZONTAL + 10, dialogueBoxY, currentDialogueItem->text, 256);
  }

}


