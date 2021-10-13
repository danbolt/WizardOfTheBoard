
#include "dialogue.h"

#include "graphic.h"
#include "dialogue/dialoguelookup.h"
#include "main.h"
#include "nustdfuncs.h"
#include "segmentinfo.h"
#include "sixtwelve.h"
#include "sixtwelve_helpers.h"

u32 dialogueState;

static int bipIndex;
static float bipTimePassed;

#define BIP_TIME_SECONDS 0.1f

typedef union {
  DialogueItem item;
  u8 aligner[320]; // padding to ensure DMA alignment
} DMAAlignedDialogueItem;

#define NUMBER_OF_DIALOGUE_ITEM_BUFFERS 3
static DMAAlignedDialogueItem dialogueItemTripleBuffer[NUMBER_OF_DIALOGUE_ITEM_BUFFERS] __attribute__((aligned(8)));
static int nextDialogueItemIndex;
static DialogueItem* currentDialogueItem;

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
  dialogueState = DIALOGUE_STATE_OFF;
  bipIndex = 0;
  nextDialogueItemIndex = 0;
  currentDialogueItem = NULL;
}

void startDialogueItem(u32 offset) {
  bipIndex = 0;
  bipTimePassed = 0.f;

  DialogueItem* nextDialogueItem = &(dialogueItemTripleBuffer[nextDialogueItemIndex].item);
  nextDialogueItemIndex = (nextDialogueItemIndex + 1) % NUMBER_OF_DIALOGUE_ITEM_BUFFERS;

  nuPiReadRom((u32)(_dialogue_dataSegmentRomStart + offset), (void*)(nextDialogueItem), sizeof(DialogueItem));

  currentDialogueItem = nextDialogueItem;

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
    if (contdata[0].trigger & B_BUTTON) {
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

  gDPPipeSync(glistp++);
  gDPLoadTextureBlock_4b(glistp++, sixtwelve_tex, G_IM_FMT_IA, SIXTWELVE_TEXTURE_WIDTH, SIXTWELVE_TEXTURE_HEIGHT, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  drawString(TITLE_SAFE_HORIZONTAL, 64, currentDialogueItem->text, (SCREEN_WD - (TITLE_SAFE_HORIZONTAL * 2)));
}


