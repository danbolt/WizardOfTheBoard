#ifndef DISPLAY_TEXT_H
#define DISPLAY_TEXT_H

#include "graphic.h"

#define DISPLAY_FONT_LETTER_WIDTH 13
#define DISPLAY_FONT_LETTER_HEIGHT 16

extern u8 displayTextTexture[TMEM_SIZE_BYTES] __attribute__((aligned(8)));

void loadDisplayText();

int measureDisplayText(const char* text);

// You're responsible for the TMEM load before calling this function:
// gDPLoadTextureBlock_4b(glistp++, OS_K0_TO_PHYSICAL(displayTextTexture), G_IM_FMT_IA, 512, 16, 0, G_TX_NOMIRROR, G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
void renderDisplayText(int x, int y, const char* text);

#endif