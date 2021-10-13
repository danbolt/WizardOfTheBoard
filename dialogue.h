

#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "nusys.h"

#define DIALOGUE_STATE_OFF 0 
#define DIALOGUE_STATE_SHOWING 1
extern u32 dialogueState;

typedef struct {
  // a string key to an actor name
  u8 speaker[16];

  // The "words" of the text
  u8 text[256];

  // Offset to the next piece of dialogue, 0x0 if this is the last piece of dialogue
  u32 nextAddress; // 0x0 to end dialogue

  u8 flags[32]; // reserved data for thinks like branching/flags/sounds/etc.
} DialogueItem;

void initalizeDialogue();

void startDialogue(const char* key);
void updateDialogue();

void renderDialogueToDisplayList();


#endif