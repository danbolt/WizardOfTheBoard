
#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <nusys.h>

typedef struct {
  unsigned char dialogue[16];

  unsigned char imageKey1[16];
  unsigned char imageKey2[16];
  unsigned char imageKey3[16];

  int bgmIndex;
} CutsceneInfo;

// Set this before calling `initCutscene`!
extern const char* cutsceneToLoad;

extern u8 backgroundIndex;

void initCutscene();
void makeCutsceneDisplaylist();
void updateCutscene();

#endif