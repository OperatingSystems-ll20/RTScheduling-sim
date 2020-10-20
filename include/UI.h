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

typedef void (*funcPtr)(struct nk_context *pNKcontext, Martian *pMartian);

Options *_mainOptions;

void initUI(Options *pOptions);
void setCustomStyle(struct nk_context *pNKcontext);

void drawMenu(struct nk_context *pNKcontext, MazeBounds pMazeBounds);

void martianHUD(struct nk_context *pNKcontext, Martian *pMartian);
void drawMartianHUD(struct nk_context *pNKcontext, MazeBounds pMazeBounds, DynamicArray *pMartians, DynamicArray *pHUDfunctions);

#endif
