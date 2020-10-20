#include <stdio.h>
#include <consts.h>
#include <UI.h>

#define NK_IMPLEMENTATION
#define NK_ALLEGRO5_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_allegro5.h>

void initUI(Options *pOptions){
    _mainOptions = pOptions;
}

void setCustomStyle(struct nk_context *pNKcontext){
    struct nk_color table[NK_COLOR_COUNT];
    table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
    table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
    table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(95, 155, 252, 245);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(95, 155, 252, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(95, 155, 252, 255);
    // table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
    // table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
    // table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
    table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
    nk_style_from_table(pNKcontext, table);
}


void drawMenu(struct nk_context *pNKcontext, MazeBounds pMazeBounds){
    if (nk_begin(pNKcontext, "Menu", nk_rect(0, 0, SCREEN_WIDHT, pMazeBounds.y0-50),
        NK_WINDOW_BORDER))
    {

        nk_layout_row_dynamic(pNKcontext, 30, 3);
        nk_checkbox_label(pNKcontext, "HUD", &_mainOptions->_showHUD);
        nk_checkbox_label(pNKcontext, "Show martians", &_mainOptions->_showMartians);
        nk_checkbox_label(pNKcontext, "Show martians position", &_mainOptions->_showMartianPos);

    }
    nk_end(pNKcontext);
}


void martianHUD(struct nk_context *pNKcontext, Martian *pMartian){
    int MYtitlebar = nk_true;
    float ratio1[] = {0.3f, 0.7f};
    struct nk_color workingColor = {0, 255, 0, 255};
    if (nk_tree_push_id(pNKcontext, NK_TREE_TAB, pMartian->title, NK_MAXIMIZED, pMartian->id)) {
        if(_mainOptions->_showMartianPos){
            nk_layout_row_dynamic(pNKcontext, 30, 2);
            nk_labelf(pNKcontext, NK_TEXT_LEFT, "PosX: %zu" , pMartian->posX);
            nk_labelf(pNKcontext, NK_TEXT_LEFT, "PosY: %zu" , pMartian->posY);
        }
        nk_layout_row_dynamic(pNKcontext, 30, 2);
        nk_label(pNKcontext, "Moving:", NK_TEXT_LEFT);
        if(!pMartian->doWork) {
            workingColor.r = (nk_byte)255;
            workingColor.g = (nk_byte)0;
        }
        nk_button_color(pNKcontext, workingColor);

        nk_layout_row(pNKcontext, NK_DYNAMIC, 30, 2, ratio1);
        nk_labelf(pNKcontext, NK_TEXT_LEFT, "Energy: %zu" , pMartian->currentEnergy);
        nk_progress(pNKcontext, &pMartian->currentEnergy, pMartian->maxEnergy, NK_FIXED);

        nk_tree_pop(pNKcontext);
    }
}


void drawMartianHUD(struct nk_context *pNKcontext, MazeBounds pMazeBounds,
                    DynamicArray *pMartians, DynamicArray *pHUDfunctions){
    if (nk_begin(pNKcontext, "Martians status",
        nk_rect(pMazeBounds.x1+10, pMazeBounds.y0, SCREEN_WIDHT - pMazeBounds.x1-20, MAZE_HEIGHT-10),
        NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        //Show info of all martians
        for(int i=0; i<pMartians->elements; i++)
            ((funcPtr)(pHUDfunctions->array[i]))(pNKcontext, (Martian*)pMartians->array[i]);
    }
    nk_end(pNKcontext);
}
