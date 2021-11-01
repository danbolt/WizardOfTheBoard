

#include <nusys.h>
#include "main.h"

#include "dialogue.h"
#include "displaytext.h"
#include "cutscene.h"
#include "gameaudio.h"
#include "levelselect.h"
#include "titlescreen.h"

#include <segmentinfo.h>

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
ScreenInfo titleScreenStage = {
  initTitleScreen,
  updateTitleScreen,
  makeTitleScreenDL
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
u8 flashingProjectiles;

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

  loadDisplayText();

  cutsceneToLoad = "test_scene";

  currentLevel = 0;

  ingameFOV = FOV_60;
  flashingProjectiles = 1;

  time = OS_CYCLES_TO_USEC(osGetTime());
  updateTime();

  initalizeDialogue();
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

#ifdef PAL_ROM
void callback_prenmi()
{
    osViSetYScale(1);
    nuGfxDisplayOff();
}

#endif

void mainproc(void)
{
#ifdef PAL_ROM
  osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);
  osViSetYScale(0.833);
  nuPreNMIFuncSet((NUScPreNMIFunc)callback_prenmi);
#endif

  initalizeGameData();
  
  /* The initialization of graphic  */
  nuGfxInit();

  /* The initialization of the controller manager  */
  contPattern = nuContInit();

  initializeAudio();

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
