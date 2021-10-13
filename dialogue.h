

#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "nusys.h"

#define DIALOGUE_STATE_OFF 0 
#define DIALOGUE_STATE_SHOWING 1
extern u32 dialogueState;

void initalizeDialogue();

void startDialogue(const char* key);
void updateDialogue();

void renderDialogueToDisplayList();


#endif