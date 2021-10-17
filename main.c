

#include <nusys.h>
#include "main.h"

#include "dialogue.h"

#include <segmentinfo.h>

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

/* Declaration of the prototype  */
void stage00(int);

/* Declaration of the external function  */
void initStage00(void);
void makeDL00(void);
void updateGame00(void);

/* The global variable  */
NUContData	contdata[1]; /* Read data of 1 controller  */
u8 contPattern;		     /* The pattern connected to the controller  */

OSTime time = 0;
OSTime delta = 0;
float deltaTimeSeconds;

float ingameFOV;

void updateTime() {
  OSTime newTime = OS_CYCLES_TO_USEC(osGetTime());
  delta = newTime - time;
  time = newTime;
  deltaTimeSeconds = delta * 0.000001f;
}

void initalizeGameData() {
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

  /* The initialization for stage00()  */
  initStage00();
  /* Register call-back  */
  nuGfxFuncSet((NUGfxFunc)stage00);
  /* The screen display is ON */
  nuGfxDisplayOn();

  while(1)
    ;
}

/*-----------------------------------------------------------------------------
  The call-back function 

  pendingGfx which is passed from Nusystem as the argument of the call-back 
  function is the total of RCP tasks that are currently processing and 
  waiting for the process. 
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  updateTime();
  updateDialogue();

  /* Provide the display process if 2 or less RCP tasks are processing or
	waiting for the process.  */
  if(pendingGfx < 3)
    makeDL00();		

  /* The process of game progress  */
  updateGame00(); 
}

