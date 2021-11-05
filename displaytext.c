
#include <nusys.h>

#include "displaytext.h"
#include "graphic.h"
#include "segmentinfo.h"

u8 displayTextTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

void loadDisplayText() {
  nuPiReadRom((u32)(_display_textSegmentRomStart), (void*)(displayTextTexture), TMEM_SIZE_BYTES);
}

int measureDisplayText(const char* text) {
  int sum = 0;

  for (int i = 0; text[i] != '\0'; i++) {
    sum += DISPLAY_FONT_LETTER_WIDTH;

    if (text[i] == 'I') {
      sum -= 6;
    } else if (text[i] == ' ') {
      sum -= (DISPLAY_FONT_LETTER_WIDTH - 6);
    }
  }

  return sum;
}

void renderDisplayText(int x, int y, const char* text) {
  int advance = x;
  int ySpot = y;
  for (int i = 0; text[i] != '\0'; i++) {

    const char letter = text[i];
    u32 s = 0;
    if ((letter >= 65) && (letter <= 89)) {
      s = ((letter - 65) + 2) * DISPLAY_FONT_LETTER_WIDTH;
    } else if ((letter >= 48) && (letter <= 57)) {
      s = (((letter - 48)) * DISPLAY_FONT_LETTER_WIDTH) + 364;
    } else if (letter == '!') {
      s = 13;
    } else if (letter == '\n') {
      ySpot += DISPLAY_FONT_LETTER_HEIGHT;
      advance = x;
      continue;
    } else if (letter == ' ') {
      advance += 6;
      continue;
    }
    gSPScisTextureRectangle(glistp++, (advance) << 2, (ySpot) << 2, (advance + DISPLAY_FONT_LETTER_WIDTH) << 2, (ySpot + DISPLAY_FONT_LETTER_HEIGHT - 1) << 2, 0, s << 5, 0 << 5, 1 << 10, 1 << 10);
    advance += DISPLAY_FONT_LETTER_WIDTH;

    if (letter == 'I') {
      advance -= 6;
    } else if (s == 0) {
      advance -= 8;
    }
  }
}