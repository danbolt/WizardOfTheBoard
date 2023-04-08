#ifndef STAGEKEYS_H
#define STAGEKEYS_H

#include <nusys.h>

typedef struct {
  const char* levelKey;
  unsigned char bgmTrack;
  const char* completionCutsceneKey;
} LevelEntry;

// TODO: maybe we could try some preprocessor trickery to automate this?
#define NUMBER_OF_LEVELS 17
extern LevelEntry levels[];

extern float best_times[NUMBER_OF_LEVELS];
extern u32 best_move_count[NUMBER_OF_LEVELS];

#endif