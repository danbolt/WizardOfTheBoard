

#ifndef MAIN_H
#define MAIN_H

#ifdef _LANGUAGE_C

extern volatile u32 changeScreensFlag;

extern u32 currentLevel;

/* Definition of the external variable  */
extern NUContData	contdata[1]; /* Read data of the controller  */
extern u8 contPattern;		     /* The pattern of the connected controller  */

// Setting for the ingame FOV
extern float ingameFOV;

// Frame delta
extern OSTime time;
extern OSTime delta;
extern float deltaTimeSeconds;

typedef void (*InitCallback)();
typedef void (*UpdateCallback)();
typedef void (*MakeDLCallback)();

typedef struct {
  InitCallback init;
  UpdateCallback update;
  MakeDLCallback makeDL;
} ScreenInfo;

extern ScreenInfo gameplayStage;
extern ScreenInfo levelSelectStage;
extern ScreenInfo cutsceneStage;

extern volatile ScreenInfo* nextStage;

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




