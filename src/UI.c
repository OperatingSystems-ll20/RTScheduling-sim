#include <stdio.h>
#include <consts.h>
#include <UI.h>

#define NK_IMPLEMENTATION
#define NK_ALLEGRO5_IMPLEMENTATION
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_allegro5.h>

/* Size bounds of the widgets */
static struct nk_rect MENU_BOUNDS = {0, 0, SCREEN_WIDHT, 40};
static struct nk_rect SIM_TIME_BOUNDS = {SIM_TIME_POS_X, SIM_TIME_HEIGHT, SIM_TIME_WIDTH, SIM_TIME_HEIGHT};
static struct nk_rect POPUP_BOUNDS = {SCREEN_WIDHT/2 - 110, SCREEN_HEIGHT/2 - 45, 220, 90};
static struct nk_rect STOP_SIM_BOUNDS = {SCREEN_WIDHT/2 - 110, SCREEN_HEIGHT/2 - 55, 220, 110};
static struct nk_rect MANUAL_SELECTION_BOUNDS = {(SCREEN_WIDHT/2) - 130, (SCREEN_HEIGHT/2) - 125, 260, 250};
static struct nk_rect POPUP_NEW_SIM_BOUNDS = {SCREEN_WIDHT/2 - 150, SCREEN_HEIGHT/2 - 80, 300, 160};
static struct nk_rect AUTOMATIC_OP_MENU_BOUNDS = {SCREEN_WIDHT/2 - 200, SCREEN_HEIGHT/2 - 225, 400, 450};
static struct nk_rect REPORT_WINDOW_BOUNDS = {SCREEN_WIDHT/2 - 275, SCREEN_HEIGHT/2 - 80, 550, 160};

/**
 * @brief Sets a custom set of color for the Nuklear UI
 * 
 * @param pNKcontext Struct nk_context
 */
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

/**
 * @brief Draws the menu bar on top of the window
 * 
 * @param pNKcontext Struct nk_context
 */
void drawMenu(struct nk_context *pNKcontext){
    if (nk_begin(pNKcontext, "Menu", MENU_BOUNDS, NK_WINDOW_BORDER)) {

        nk_menubar_begin(pNKcontext);
        /* Menu */
        nk_layout_row_begin(pNKcontext, NK_STATIC, 25, 5);
        nk_layout_row_push(pNKcontext, 45);
        if (nk_menu_begin_label(pNKcontext, "Menu", NK_TEXT_LEFT, nk_vec2(150, 200))) {
            float ratio[] = {0.9f, 0.1f};
            nk_layout_row_dynamic(pNKcontext, 25, 1);
            if (nk_menu_item_label(pNKcontext, "New simulation", NK_TEXT_LEFT))
                _options._newSimulationPopUp = 1;

            
            if(_options._operationMode == MANUAL && !_options._stopSimulation){
                nk_layout_row(pNKcontext, NK_DYNAMIC, 25, 2, ratio);
                if (nk_menu_item_label(pNKcontext, "New Martian", NK_TEXT_LEFT))
                    _options._newMartianMenu = 1;
                nk_label(pNKcontext, "N", NK_TEXT_RIGHT);
            }
            if(_options._operationMode != UNDEFINED){
                nk_layout_row(pNKcontext, NK_DYNAMIC, 25, 2, ratio);
                if (nk_menu_item_label(pNKcontext, "Pause", NK_TEXT_LEFT))
                    _options._pause = 1;
                nk_label(pNKcontext, "P", NK_TEXT_RIGHT);
            }
            if(_options._operationMode != UNDEFINED){
                nk_layout_row(pNKcontext, NK_DYNAMIC, 25, 2, ratio);
                if (nk_menu_item_label(pNKcontext, "Stop", NK_TEXT_LEFT)){
                    if(!_options._stopSimulation){
                        _options._pause = 1;
                        _options._showStopSimWarning = 1;
                    }
                }
                nk_label(pNKcontext, "S", NK_TEXT_RIGHT);
            }

            nk_layout_row_dynamic(pNKcontext, 25, 1);
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


/**
 * @brief Draws the simulation time on screen and the buttons for show and save the report
 * 
 * @param pNKcontext Struct nk_context
 * @param pSecTimer  Current second timer
 * @param pTicks     Current tick count 
 */
void showSimTime(struct nk_context *pNKcontext, const int pSecTimer, const int pTicks){
    if (nk_begin(pNKcontext, "Simulation Time", SIM_TIME_BOUNDS, 0)) {
        nk_layout_row_dynamic(pNKcontext, 25, 1);
        nk_label(pNKcontext, "Simulation time:", NK_TEXT_CENTERED);
        nk_labelf(pNKcontext, NK_TEXT_CENTERED, "%.0f s" , pSecTimer + (1.0/REFRESH_RATE)*pTicks);

        if(_options._stopSimulation) {
            SIM_TIME_BOUNDS.h = 115;
            nk_layout_row_dynamic(pNKcontext, 10, 1);
            if(_options._saveReport == 0){
                nk_layout_row_dynamic(pNKcontext, 25, 2);
                if (nk_button_label(pNKcontext, "Show report"))
                    _options._showReport = 1;
                if(nk_button_label(pNKcontext, "Save"))
                    _options._saveReport = 1;
            }
            else{
                nk_layout_row_dynamic(pNKcontext, 25, 1);
                if (nk_button_label(pNKcontext, "Show report"))
                    _options._showReport = 1;
            }
        }
    }
    nk_end(pNKcontext);
}

/**
 * @brief Draws a popup on screen
 * 
 * @param pNKcontext  Struct nk_context
 * @param pTitle      Title of the popup window (invisible, used only for id)
 * @param pOption     Option toggle by popup button
 * @param pMessage    Message of the popup
 * @param pButtonTxt  Text of the button
 */
void showPopUp(struct nk_context *pNKcontext, const char *pTitle, 
                int *pOption, const char *pMessage, const char *pButtonTxt)
{
    if(*pOption) {
        if (nk_begin(pNKcontext, pTitle, nk_rect(0, 0, 0, 0), NK_WINDOW_BORDER)) {
            if (nk_popup_begin(pNKcontext, NK_POPUP_STATIC, "Small popUp", 0, POPUP_BOUNDS)) {
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

/**
 * @brief Draws a popup on screen warning about stopping simulation
 * 
 * @param pNKcontext Struct nk_context
 */
void stopSimMenu(struct nk_context *pNKcontext){
    if(_options._showStopSimWarning) {
        if (nk_begin(pNKcontext, "Stop simulation", nk_rect(0, 0, 0, 0), NK_WINDOW_BORDER)) {
            if (nk_popup_begin(pNKcontext, NK_POPUP_STATIC, "Small popUp", 0, STOP_SIM_BOUNDS)) {
                nk_layout_row_dynamic(pNKcontext, 50, 1);
                nk_label_wrap(pNKcontext, STOP_SIM_WARNING);

                nk_layout_row_dynamic(pNKcontext, 25, 2);
                if (nk_button_label(pNKcontext, "Yes")) {
                    _options._stopSimulation = 1;
                    _options._showStopSimWarning = 0;
                    _options._pause = 0;
                    nk_popup_close(pNKcontext);
                }
                if(nk_button_label(pNKcontext, "No")){
                    _options._pause = 0;
                    _options._showStopSimWarning = 0;
                    nk_popup_close(pNKcontext);
                }
                nk_popup_end(pNKcontext);
            }
            else _options._showStopSimWarning = 0;
        }
        nk_end(pNKcontext);
    }
}


/**
 * @brief Draws a widget on screen showing the report image
 * 
 * @param pNKcontext Struct nk_context
 * @param pReportImg Report image (Allegro bitmap)
 */
void showReportWindow(struct nk_context *pNKcontext, struct nk_image pReportImg){
    if(_options._showReport) {
        if (nk_begin(pNKcontext, "Report", REPORT_WINDOW_BOUNDS, NK_WINDOW_CLOSABLE|NK_WINDOW_BORDER
            |NK_WINDOW_MOVABLE)) 
        {
            nk_layout_row_static(pNKcontext, pReportImg.h, pReportImg.w, 1);
            nk_image(pNKcontext, pReportImg);
        }
        else _options._showReport = 0;
        nk_end(pNKcontext);
    }
}


/**
 * @brief Draws the widget with the menu for the selection of scheduling algorithm
 *        when manual mode is chosen
 * 
 * @param pNKcontext 
 */
void manualModeScheduleSelection(struct nk_context *pNKcontext) {
    if(_options._prepareManualSim) {
        if (nk_begin(pNKcontext, "Manual operation mode", MANUAL_SELECTION_BOUNDS, NK_WINDOW_BORDER|
        NK_WINDOW_CLOSABLE|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR)) 
        {

            nk_layout_row_dynamic(pNKcontext, 25, 1);
            nk_label(pNKcontext, "Choose the scheduling algorithm", NK_TEXT_CENTERED);

            nk_layout_space_begin(pNKcontext, NK_STATIC, 25, 2);
            nk_layout_space_push(pNKcontext, nk_rect(40,0,60,25));
            _options._schedulingAlgorithm = nk_option_label(pNKcontext, "RM",
                _options._schedulingAlgorithm == RM) ? RM : _options._schedulingAlgorithm;
            nk_layout_space_push(pNKcontext, nk_rect(125,0,40,25));
            _options._schedulingAlgorithm = nk_option_label(pNKcontext, "EDF",
                _options._schedulingAlgorithm == EDF) ? EDF : _options._schedulingAlgorithm;
            nk_layout_space_end(pNKcontext);

            nk_layout_space_begin(pNKcontext, NK_STATIC, 25, 1);
            nk_layout_space_push(pNKcontext, nk_rect(60,0,130,25));
            if (nk_button_label(pNKcontext, "Start simulation")) {
                _options._operationMode = MANUAL;
                _options._startSimulation = 1;
                _options._showSimTime = 1;
                _options._showHUD = 1;
                _options._prepareManualSim = 0;
            }
            nk_layout_space_end(pNKcontext);

            nk_layout_row_dynamic(pNKcontext, 25, 1);
            nk_layout_row_dynamic(pNKcontext, 60, 1);
            nk_label_wrap(pNKcontext, ADD_MARTIAN_INFO);
        }
        else _options._prepareManualSim = 0;
        nk_end(pNKcontext);
    }
}


/**
 * @brief Draws a widget with the menu for selection of operation mode
 * 
 * @param pNKcontext Struct nk_context
 */
void newSimMenu(struct nk_context *pNKcontext){
    if(_options._newSimulationPopUp) {
        if (nk_begin(pNKcontext, "New Simulation", POPUP_NEW_SIM_BOUNDS, NK_WINDOW_BORDER|NK_WINDOW_CLOSABLE)) {

            struct nk_rect bounds;
            const struct nk_input *in = &pNKcontext->input;
            nk_layout_row_dynamic(pNKcontext, 25, 1);
            nk_label(pNKcontext, "Choose operation mode:", NK_TEXT_CENTERED);

            nk_layout_row_dynamic(pNKcontext, 25, 2);
            if (nk_button_label(pNKcontext, "Manual")) {
                // _options._manualOpMenu = 1;
                _options._prepareManualSim = 1;
                _options._newSimulationPopUp = 0;
            }
            if (nk_button_label(pNKcontext, "Automatic")) {
                _options._automaticOpMenu = 1;
                _options._newSimulationPopUp = 0;
            }

            nk_layout_row_dynamic(pNKcontext, 10, 1);
            nk_layout_row_dynamic(pNKcontext, 25, 1);
            bounds = nk_widget_bounds(pNKcontext);
            nk_label(pNKcontext, "About modes", NK_TEXT_RIGHT);
            if (nk_input_is_mouse_hovering_rect(in, bounds)){
                nk_tooltip_begin(pNKcontext, 230);
                nk_layout_row_static(pNKcontext, 55, 200, 1);
                nk_label_wrap(pNKcontext, ABOUT_MANUAL_MODE);
                nk_label_wrap(pNKcontext, ABOUT_AUTOMATIC_MODE);
                nk_tooltip_end(pNKcontext);
            }
        }
        else _options._newSimulationPopUp = 0;
        nk_end(pNKcontext);
    }
}


/**
 * @brief Draws a widget with the menu for automatic mode, allowing the addition of martians
 *        and the selection of the scheduling algorithm
 * 
 * @param pNKcontext Struct nk_context
 */
void automaticOpMenu(struct nk_context *pNKcontext){
    if(_options._automaticOpMenu) {
        if (nk_begin(pNKcontext, "Automatic operation mode", AUTOMATIC_OP_MENU_BOUNDS, NK_WINDOW_BORDER|NK_WINDOW_CLOSABLE|
            NK_WINDOW_MOVABLE)) {

            static int martianCounter = 0;
            static int energyProp = 2;
            static int periodProp = 6;
            nk_layout_row_begin(pNKcontext, NK_STATIC, 25, 3);
            nk_layout_row_push(pNKcontext, 100);
            nk_label(pNKcontext, "New martian:", NK_TEXT_LEFT);
            nk_layout_row_push(pNKcontext, 130);
            nk_property_int(pNKcontext, "#Energy:", 1, &energyProp, 20, 1, 1);
            nk_layout_row_push(pNKcontext, 130);
            nk_property_int(pNKcontext, "#Period:", 1, &periodProp, 40, 1, 1);
            if(periodProp <= energyProp) periodProp = energyProp;
            nk_layout_row_end(pNKcontext);

            nk_layout_space_begin(pNKcontext, NK_STATIC, 25, 1);
            nk_layout_space_push(pNKcontext, nk_rect(300,0,60,25));
            if (nk_button_label(pNKcontext, "Add")) {
                if(martianCounter < MAX_MARTIANS){
                    _options._newMartians[martianCounter].energy = energyProp;
                    _options._newMartians[martianCounter].period = periodProp;
                    martianCounter++;
                }
            }
            nk_layout_space_end(pNKcontext);

            if(martianCounter > 0) {
                nk_layout_row_dynamic(pNKcontext, 250, 1);
                if (nk_group_begin(pNKcontext, "Martian List", 0)) {
                    nk_layout_row_dynamic(pNKcontext, 25, 3);
                    nk_label(pNKcontext, "Martian", NK_TEXT_CENTERED);
                    nk_label(pNKcontext, "Energy", NK_TEXT_CENTERED);
                    nk_label(pNKcontext, "Period", NK_TEXT_CENTERED);
                    for (int i = 0; i < martianCounter; i++){
                        nk_labelf(pNKcontext, NK_TEXT_CENTERED, "Martian %d" , i);
                        nk_labelf(pNKcontext, NK_TEXT_CENTERED, "%d" , _options._newMartians[i].energy);
                        nk_labelf(pNKcontext, NK_TEXT_CENTERED, "%d" , _options._newMartians[i].period);
                    }
                    nk_group_end(pNKcontext);
                }

                nk_layout_row_dynamic(pNKcontext, 10, 3); //Space
                float ratio[] = {0.5f, 0.25f, 0.25f};
                nk_layout_row(pNKcontext, NK_DYNAMIC, 25, 3, ratio);
                nk_label(pNKcontext, "Scheduling algorithm:", NK_TEXT_LEFT);
                _options._schedulingAlgorithm = nk_option_label(pNKcontext, "RM",
                    _options._schedulingAlgorithm == RM) ? RM : _options._schedulingAlgorithm;
                _options._schedulingAlgorithm = nk_option_label(pNKcontext, "EDF",
                    _options._schedulingAlgorithm == EDF) ? EDF : _options._schedulingAlgorithm;

                nk_layout_row_dynamic(pNKcontext, 10, 1); //Space
                nk_layout_space_begin(pNKcontext, NK_STATIC, 25, 1);
                nk_layout_space_push(pNKcontext, nk_rect(220,0,130,25));
                if (nk_button_label(pNKcontext, "Start simulation")) {
                    _martianAmount = martianCounter;
                    martianCounter = 0;
                    _options._prepareAutomaticSim = 1;
                    _options._operationMode = AUTOMATIC;
                    _options._automaticOpMenu = 0;
                }
                nk_layout_space_end(pNKcontext);
            }
        }
        else _options._automaticOpMenu = 0;
        nk_end(pNKcontext);
    }
}


/**
 * @brief Draws a widget with the menu for adding new martians in the manual operation mode
 * 
 * @param pNKcontext Struct nk_context
 */
void addMartianMenu(struct nk_context *pNKcontext){
    if(_options._newMartianMenu) {
        if (nk_begin(pNKcontext, "New martian", 
            nk_rect(10, _mazeBounds.y0, _mazeBounds.x0-20, 190), 
            NK_WINDOW_BORDER|NK_WINDOW_CLOSABLE/*|NK_WINDOW_MOVABLE*/)) 
        {
            static int energyProp = 2;
            static int periodProp = 6;
            nk_layout_row_dynamic(pNKcontext, 25, 1);
            nk_label(pNKcontext, "New martian:", NK_TEXT_CENTERED);
            nk_property_int(pNKcontext, "#Energy:", 1, &energyProp, 20, 1, 1);
            nk_property_int(pNKcontext, "#Period:", 1, &periodProp, 40, 1, 1);
            if(periodProp <= energyProp) periodProp = energyProp;

            nk_layout_row_dynamic(pNKcontext, 10, 1); //Space
            nk_layout_space_begin(pNKcontext, NK_STATIC, 25, 1);
            struct nk_rect r = nk_layout_space_bounds(pNKcontext);
            nk_layout_space_push(pNKcontext, nk_rect((r.w/2)-30,0,60,25));
            
            if (nk_button_label(pNKcontext, "Add")) {
                    _options._newMartians[0].energy = energyProp;
                    _options._newMartians[0].period = periodProp;
                    _options._insertNewMartian = 1;
                    _options._newMartianMenu = 0;
            }
            nk_layout_space_end(pNKcontext);
        }
        else _options._newMartianMenu = 0;
        nk_end(pNKcontext);
    }
}


/**
 * @brief Draws the information of one martian on screen
 * 
 * @param pNKcontext Struct nk_context
 * @param pMartian   Structure of type Martian holding all its data
 */
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
        nk_layout_row(pNKcontext, NK_DYNAMIC, 30, 2, ratio1);
        nk_labelf(pNKcontext, NK_TEXT_LEFT, "Period: %d" , pMartian->period);

        nk_tree_pop(pNKcontext);
    }
}


/**
 * @brief Draws the widget where the information of all martian is displayed
 * 
 * @param pNKcontext     Struct nk_context
 * @param pMartians      DynamicArray of Martian structures
 * @param pHUDfunctions  DynamicArray of function pointer
 */
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
