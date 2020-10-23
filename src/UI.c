#include <stdio.h>
#include <consts.h>
#include <UI.h>

#define NK_IMPLEMENTATION
#define NK_ALLEGRO5_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_allegro5.h>

static struct nk_rect MENU_BOUNDS = {0, 0, SCREEN_WIDHT, 40};
static struct nk_rect SIM_TIME_BOUNDS = {SIM_TIME_POS_X, SIM_TIME_HEIGHT, SIM_TIME_WIDTH, SIM_TIME_HEIGHT};
static struct nk_rect POPUP_BOUNDS = {SCREEN_WIDHT/2 - 110, SCREEN_HEIGHT/2 - 45, 220, 90};

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

void drawMenu(struct nk_context *pNKcontext){
    static int show_menu = nk_true;
    if (nk_begin(pNKcontext, "Menu", MENU_BOUNDS, NK_WINDOW_BORDER)) {

        nk_menubar_begin(pNKcontext);
        /* Menu */
        nk_layout_row_begin(pNKcontext, NK_STATIC, 25, 5);
        nk_layout_row_push(pNKcontext, 45);
        if (nk_menu_begin_label(pNKcontext, "Menu", NK_TEXT_LEFT, nk_vec2(200, 200))) {
            float ratio[] = {0.9f, 0.1f};
            nk_layout_row(pNKcontext, NK_DYNAMIC, 25, 2, ratio);
            if (nk_menu_item_label(pNKcontext, "New Martian", NK_TEXT_LEFT))
                show_menu = nk_false;
            nk_label(pNKcontext, "N", NK_TEXT_RIGHT);
            if (nk_menu_item_label(pNKcontext, "Pause", NK_TEXT_LEFT))
                _options._pause = 1;
            nk_label(pNKcontext, "P", NK_TEXT_RIGHT);
            if (nk_menu_item_label(pNKcontext, "Exit", NK_TEXT_LEFT))
                _options._exit = 1;
            nk_menu_end(pNKcontext);
        }

        /* Options */
        nk_layout_row_push(pNKcontext, 60);
        if (nk_menu_begin_label(pNKcontext, "Options", NK_TEXT_LEFT, nk_vec2(200, 600))) {
            // float ratio[] = {0.9f, 0.1f};
            nk_layout_row_dynamic(pNKcontext, 25, 1);
            nk_checkbox_label(pNKcontext, "Show martians", &_options._showMartians);
            nk_checkbox_label(pNKcontext, "Show martian info", &_options._showHUD);
            nk_checkbox_label(pNKcontext, "Show positions", &_options._showMartianPos);
            nk_layout_row_dynamic(pNKcontext, 25, 2);
            nk_labelf(pNKcontext, NK_TEXT_LEFT, "Speed: %d" , _options._martianSpeed);
            nk_slider_int(pNKcontext, 2, &_options._martianSpeed, 12, 1);
            nk_menu_end(pNKcontext);
        }
        nk_menubar_end(pNKcontext);
    }
    nk_end(pNKcontext);
}

void showSimTime(struct nk_context *pNKcontext, const int pSecTimer, const int pTicks){
    if (nk_begin(pNKcontext, "Simulation Time", SIM_TIME_BOUNDS, NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(pNKcontext, 25, 1);
        nk_label(pNKcontext, "Simulation time:", NK_TEXT_CENTERED);
        nk_labelf(pNKcontext, NK_TEXT_CENTERED, "%.0f s" , pSecTimer + (1.0/REFRESH_RATE)*pTicks);
    }
    nk_end(pNKcontext);
}

void showPopUp(struct nk_context *pNKcontext, const char *pTitle, int *pOption, const char *pMessage, const char *pButtonTxt){
    if(*pOption) {
        if (nk_begin(pNKcontext, pTitle, nk_rect(0, 0, 0, 0), NK_WINDOW_BORDER)) {
            static struct nk_rect s = {SCREEN_WIDHT/2 - 110, SCREEN_HEIGHT/2 - 45, 220, 90};
            if (nk_popup_begin(pNKcontext, NK_POPUP_STATIC, "Pause", 0, POPUP_BOUNDS)) {
                nk_layout_row_dynamic(pNKcontext, 25, 1);
                nk_label(pNKcontext, pMessage, NK_TEXT_LEFT);
                nk_layout_space_begin(pNKcontext, NK_STATIC, 25, 2);

                nk_layout_space_push(pNKcontext, nk_rect(78-11,0,60,25));
                if (nk_button_label(pNKcontext, pButtonTxt)) {
                    *pOption = 0;
                    nk_popup_close(pNKcontext);
                }
                nk_layout_space_end(pNKcontext);
                nk_popup_end(pNKcontext);
            }
            else *pOption = 0;
        }
        nk_end(pNKcontext);
    }
}

void martianHUD(struct nk_context *pNKcontext, Martian *pMartian){
    int MYtitlebar = nk_true;
    float ratio1[] = {0.3f, 0.7f};
    struct nk_color stateColor = {0, 0, 0, 255};
    if (nk_tree_push_id(pNKcontext, NK_TREE_TAB, pMartian->title, NK_MAXIMIZED, pMartian->id)) {
        if(_options._showMartianPos){
            nk_layout_row_dynamic(pNKcontext, 30, 2);
            nk_labelf(pNKcontext, NK_TEXT_LEFT, "PosX: %d" , pMartian->posX);
            nk_labelf(pNKcontext, NK_TEXT_LEFT, "PosY: %d" , pMartian->posY);
        }

        nk_layout_row(pNKcontext, NK_DYNAMIC, 30, 2, ratio1);
        nk_labelf(pNKcontext, NK_TEXT_LEFT, "Energy: %d" , pMartian->currentEnergy);
        nk_progress(pNKcontext, &pMartian->currentEnergy, pMartian->maxEnergy, NK_FIXED);

        nk_tree_pop(pNKcontext);
    }
}

void drawMartianHUD(struct nk_context *pNKcontext, DynamicArray *pMartians, DynamicArray *pHUDfunctions){
    if (nk_begin(pNKcontext, "Martians status",
        nk_rect(_mazeBounds.x1+10, _mazeBounds.y0, SCREEN_WIDHT - _mazeBounds.x1-20, MAZE_HEIGHT-10),
        NK_WINDOW_BORDER|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
    {
        //Show info of all martians
        for(int i=0; i<pMartians->elements; i++)
            ((funcPtr)(pHUDfunctions->array[i]))(pNKcontext, (Martian*)pMartians->array[i]);
    }
    nk_end(pNKcontext);
}
