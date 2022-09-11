#ifndef STAGEKEYS_H
#define STAGEKEYS_H


typedef struct {
  const char* levelKey;
  unsigned char bgmTrack;
  const char* completionCutsceneKey;
} LevelEntry;

// TODO: maybe we could try some preprocessor trickery to automate this?
#define NUMBER_OF_LEVELS 17
extern LevelEntry levels[];

#endif