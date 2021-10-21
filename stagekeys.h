#ifndef STAGEKEYS_H
#define STAGEKEYS_H


typedef struct {
  const char* levelKey;
} LevelEntry;

// TODO: maybe we could try some preprocessor trickery to automate this?
#define NUMBER_OF_LEVELS 3
extern LevelEntry levels[];

#endif