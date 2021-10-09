

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

/* The screen size  */
#define SCREEN_HT        240
#define SCREEN_WD        320

/* The maximum length of the display list of one task  */
#define GFX_GLIST_LEN     2048

/* The title safe area */
#define TITLE_SAFE_HORIZONTAL 22
#define TITLE_SAFE_VERTICAL 16

/* The action safe area */
#define ACTION_SAFE_HORIZONTAL 16
#define ACTION_SAFE_VERTICAL 16

/*-------------------------- define structure ------------------------------ */
/* The structure of the projection-matrix  */
typedef struct {
  Mtx ortho;
  Mtx projection;

  Mtx modelling;

  Mtx camera;
} Dynamic;

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxRCPInit(void);
extern void gfxClearCfb(void);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */



