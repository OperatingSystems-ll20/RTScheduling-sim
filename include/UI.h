#ifndef _UI_H
#define _UI_H

#include <dynamicArray.h>
#include <objects.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_allegro5.h>

static const int SIM_TIME_WIDTH = 200;
static const int SIM_TIME_HEIGHT = 80;
static const int SIM_TIME_POS_X = (SCREEN_WIDHT/2)-(SIM_TIME_WIDTH/2);

static struct nk_rect MENU_BOUNDS;
static struct nk_rect SIM_TIME_BOUNDS;
static struct nk_rect POPUP_BOUNDS;
static struct nk_rect MANUAL_SELECTION_BOUNDS;
static struct nk_rect POPUP_NEW_SIM_BOUNDS;
static struct nk_rect MARTIAN_HUD_BOUNDS;
static struct nk_rect AUTOMATIC_OP_MENU_BOUNDS;
static struct nk_rect NEW_MARTIAN_MENU_BOUNDS;

typedef void (*funcPtr)(struct nk_context *pNKcontext, Martian *pMartian);

extern Options _options;
extern MazeBounds _mazeBounds;
extern int _martianAmount;

void setCustomStyle(struct nk_context *pNKcontext);

void drawMenu(struct nk_context *pNKcontext);
void showSimTime(struct nk_context *pNKcontext, const int pSecTimer, const int pTicks);
void showPopUp(struct nk_context *pNKcontext, const char *pTitle, int *pOption, 
                const char *pMessage, const char *pButtonTxt);
void manualModeScheduleSelection(struct nk_context *pNKcontext);
void newSimMenu(struct nk_context *pNKcontext);
void automaticOpMenu(struct nk_context *pNKcontext);
void addMartianMenu(struct nk_context *pNKcontext);

void martianHUD(struct nk_context *pNKcontext, Martian *pMartian);
void drawMartianHUD(struct nk_context *pNKcontext, DynamicArray *pMartians, DynamicArray *pHUDfunctions);

#endif
