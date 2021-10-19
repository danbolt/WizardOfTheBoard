

#include <nusys.h>
#include "main.h"

#include "dialogue.h"
#include "cutscene.h"
#include "levelselect.h"

#include <segmentinfo.h>

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

// TODO: header-ify this
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

ScreenInfo gameplayStage = {
  initStage00,
  updateGame00,
  makeDL00
};
ScreenInfo levelSelectStage = {
  initLevelSelect,
  updateLevelSelect,
  makeLevelSelectDisplayList
};
ScreenInfo cutsceneStage = {
  initCutscene,
  updateCutscene,
  makeCutsceneDisplaylist
};

ScreenInfo* currentStage;
volatile ScreenInfo* nextStage;

/* The global variable  */
NUContData	contdata[1]; /* Read data of 1 controller  */
u8 contPattern;		     /* The pattern connected to the controller  */

OSTime time = 0;
OSTime delta = 0;
float deltaTimeSeconds;

float ingameFOV;

u32 currentLevel;

volatile u32 changeScreensFlag;

void updateTime() {
  OSTime newTime = OS_CYCLES_TO_USEC(osGetTime());
  delta = newTime - time;
  time = newTime;
  deltaTimeSeconds = delta * 0.000001f;
}

void initalizeGameData() {
  changeScreensFlag = 1;
  currentStage = NULL;
  nextStage = &cutsceneStage;

  cutsceneToLoad = "test_scene";

  currentLevel = 0;

  ingameFOV = 60.f;

  time = OS_CYCLES_TO_USEC(osGetTime());
  updateTime();

  initalizeDialogue();
}

void setAudioData(void)
{
  nuAuSeqPlayerBankSet(_midibankSegmentRomStart, _midibankSegmentRomEnd - _midibankSegmentRomStart, _miditableSegmentRomStart);
  nuAuSeqPlayerSeqSet(_seqSegmentRomStart);
}

void tickCurrentStage(int pendingGfx) {
  if (changeScreensFlag) {
    return;
  }

  updateTime();
  updateDialogue();

  /* Provide the display process if 2 or less RCP tasks are processing or
  waiting for the process.  */
  if(pendingGfx < 3) {
    currentStage->makeDL();   
  }

  /* The process of game progress  */
  currentStage->update(); 
}

/*------------------------
	Main
--------------------------*/
void mainproc(void)
{
  initalizeGameData();
  
  /* The initialization of graphic  */
  nuGfxInit();

  /* The initialization of the controller manager  */
  contPattern = nuContInit();

  nuAuInit();
  setAudioData();

  while (1) {
    currentStage = (ScreenInfo*)nextStage;
    nextStage = 0x0;
    changeScreensFlag = 0;

    currentStage->init();
    nuGfxFuncSet((NUGfxFunc)tickCurrentStage);
    nuGfxDisplayOn();

    while(changeScreensFlag == 0);

    nuGfxDisplayOff();
    nuGfxFuncRemove();

  }


}
