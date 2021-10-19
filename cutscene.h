
#ifndef CUTSCENE_H
#define CUTSCENE_H


typedef struct {
  unsigned char dialogue[16];

  unsigned char imageKey1[16];
  unsigned char imageKey2[16];
  unsigned char imageKey3[16];
} CutsceneInfo;

// Set this before calling `initCutscene`!
extern const char* cutsceneToLoad;

void initCutscene();
void makeCutsceneDisplaylist();
void updateCutscene();

#endif