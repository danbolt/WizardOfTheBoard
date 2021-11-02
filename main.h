

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

extern u8 flashingProjectiles;

// Frame delta
extern OSTime time;
extern OSTime delta;
extern float deltaTimeSeconds;

#define FOV_60 60.f
#define FOV_90 90.f

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
extern ScreenInfo titleScreenStage;
extern ScreenInfo betweenStagesStage;

extern volatile ScreenInfo* nextStage;

#endif /* _LANGUAGE_C */
#endif /* MAIN_H */




