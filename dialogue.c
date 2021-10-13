
#include "dialogue.h"

#include "graphic.h"
#include "main.h"
#include "sixtwelve.h"
#include "sixtwelve_helpers.h"

u32 dialogueState;

static int bipIndex;
static float bipTimePassed;

#define BIP_TIME_SECONDS 0.2f

const unsigned char* hackText = "The quick brown fox jumps over the lazy dog. Yee haw!";

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
}


void startDialogue(const char* key) {
  if (dialogueState == DIALOGUE_STATE_SHOWING) {
    return;
  }

  // TODO: DMA the dialogue in

  dialogueState = DIALOGUE_STATE_SHOWING;
  bipIndex = 0;
  bipTimePassed = 0.f;
}

void updateDialogue() {
  if (dialogueState == DIALOGUE_STATE_OFF) {
    return;
  }


  if (hackText[bipIndex] != '\0') {
  bipTimePassed += deltaTimeSeconds;
    if (bipTimePassed > BIP_TIME_SECONDS) {
      bipTimePassed = 0.f;
      bipIndex++;

    }
  }
}

void renderDialogueToDisplayList() {
  if (dialogueState == DIALOGUE_STATE_OFF) {
    return;
  }

  gDPPipeSync(glistp++);
  gDPLoadTextureBlock_4b(glistp++, sixtwelve_tex, G_IM_FMT_IA, SIXTWELVE_TEXTURE_WIDTH, SIXTWELVE_TEXTURE_HEIGHT, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
  drawString(TITLE_SAFE_HORIZONTAL, 64, hackText, (SCREEN_WD - (TITLE_SAFE_HORIZONTAL * 2)));
}


