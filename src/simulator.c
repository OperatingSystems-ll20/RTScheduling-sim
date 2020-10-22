#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <consts.h>
#include <objects.h>
#include <UI.h>
#include <schedule.h>
#include <simulator.h>

static void checkInit(bool pTest, const char *pDescription){
    if(pTest) return;
    printf("Error: Couldn't initialize %s\n", pDescription);
    exit(1);
}

static void stopAllThreads(){
    for(int i=0; i<martianAmount; i++){
        ((Martian*)_martians.array[i])->running = 0;
        ((Martian*)_martians.array[i])->update = 1;
        // ((Martian*)_martians.array[i])->doWork = 1;
        pthread_cond_signal(&((Martian*)_martians.array[i])->cond);
        pthread_join(*(pthread_t*)_threads.array[i], NULL);
    }
}

static bool checkMartianCollision(Martian *pMartian, int *pNewX, int *pNewY){
    bool collision = false;
    for(int i=0; i<martianAmount; i++){
        if(i == pMartian->id) continue; //Skip collision with itself
        Martian *other = (Martian*)_martians.array[i];

        switch(pMartian->direction){
            case LEFT:
                if( (*pNewY <= other->posY && (*pNewY+MARTIAN_SIZE) >= other->posY) ||
                    (*pNewY >= other->posY && *pNewY <= (other->posY+MARTIAN_SIZE)) )
                    {
                        if(*pNewX < (other->posX+MARTIAN_SIZE) && *pNewX > other->posX){
                            if(pMartian->posX != (other->posX+MARTIAN_SIZE)){
                                *pNewX = pMartian->posX - (pMartian->posX - (other->posX+MARTIAN_SIZE));
                                collision = false;
                            }
                            else collision = true;
                        }
                    }
                break;

            case RIGHT:
                if( (*pNewY <= other->posY && (*pNewY+MARTIAN_SIZE) >= other->posY) ||
                    (*pNewY >= other->posY && *pNewY <= (other->posY+MARTIAN_SIZE)) )
                    {
                        if((*pNewX+MARTIAN_SIZE) > other->posX &&
                          (*pNewX+MARTIAN_SIZE) < (other->posX+MARTIAN_SIZE))
                        {
                            if((pMartian->posX+MARTIAN_SIZE) != other->posX){
                                *pNewX = pMartian->posX + (other->posX - (pMartian->posX+MARTIAN_SIZE));
                                collision = false;
                            }
                            else collision = true;
                        }
                    }
                break;

            case UP:
                if( (*pNewX <= other->posX && (*pNewX+MARTIAN_SIZE) >= other->posX) ||
                    (*pNewX >= other->posX && *pNewX <= (other->posX+MARTIAN_SIZE)) )
                {
                    if(*pNewY < (other->posY+MARTIAN_SIZE) && *pNewY > other->posY) {
                        if(pMartian->posY != (other->posY+MARTIAN_SIZE)){
                            *pNewY = pMartian->posY - (pMartian->posY - (other->posY+MARTIAN_SIZE));
                            collision = false;
                        }
                        else collision = true;
                    }
                }
                break;

            case DOWN:
                if( (*pNewX <= other->posX && (*pNewX+MARTIAN_SIZE) >= other->posX) ||
                    (*pNewX >= other->posX && *pNewX <= (other->posX+MARTIAN_SIZE)) )
                {
                    if((*pNewY+MARTIAN_SIZE) > other->posY &&
                      (*pNewY+MARTIAN_SIZE) < (other->posY+MARTIAN_SIZE))
                    {
                        if((pMartian->posY+MARTIAN_SIZE) != other->posY){
                            *pNewY = pMartian->posY + (other->posY - (pMartian->posY+MARTIAN_SIZE));
                            collision = false;
                        }
                        else collision = true;
                    }
                }
                break;
        }//End switch
    }//End for
    return collision;
}

static bool checkMove(Martian *pMartian){
    int x1 = pMartian->posX + MARTIAN_SIZE;
    int y1 = pMartian->posY + MARTIAN_SIZE;
    int newX, newY;
    int relX0, relY0, relX1, relY1;
    bool validMove;

    //Keep moving until the martian is inside the maze
    if((pMartian->posX+MARTIAN_SIZE+_mazeBounds.x0) < _mazeBounds.x0){
        pMartian->posX += MARTIAN_SPEED;
        validMove = true;
        return validMove;
    }

    switch(pMartian->direction){
        case LEFT:
            newX = pMartian->posX - MARTIAN_SPEED;
            newY = pMartian->posY;
            relX0 = newX/TILE_SIZE;
            relY0 = newY/TILE_SIZE;
            relY1 = (newY+MARTIAN_SIZE-1)/TILE_SIZE;
            if(_mazeTiles[relX0][relY0].type == WALL || _mazeTiles[relX0][relY1].type == WALL) {
                if(pMartian->posX != (relX0*TILE_SIZE)+TILE_SIZE){
                    newX = pMartian->posX - (pMartian->posX - ((relX0*TILE_SIZE) + TILE_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if((newX + _mazeBounds.x0) < _mazeBounds.x0){
                if(pMartian->posX != (relX0*TILE_SIZE)){
                    newX = pMartian->posX - (pMartian->posX - (relX0*TILE_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if(checkMartianCollision(pMartian, &newX, &newY)) validMove = false;
            else validMove = true;
            break;

        case RIGHT:
            newX = pMartian->posX + MARTIAN_SPEED;
            newY = pMartian->posY;
            relX1 = (newX + MARTIAN_SIZE)/TILE_SIZE;
            relY0 = newY/TILE_SIZE;
            relY1 = (newY+MARTIAN_SIZE-1)/TILE_SIZE;
            if(_mazeTiles[relX1][relY0].type == WALL || _mazeTiles[relX1][relY1].type == WALL) {
                if((pMartian->posX + MARTIAN_SIZE) != (relX1*TILE_SIZE)){
                    newX = pMartian->posX + ((relX1*TILE_SIZE) - (pMartian->posX + MARTIAN_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if((newX+MARTIAN_SIZE+_mazeBounds.x0) > _mazeBounds.x1){
                if((pMartian->posX + MARTIAN_SIZE) != (relX1*TILE_SIZE)){
                    newX = pMartian->posX + ((relX1*TILE_SIZE) - (pMartian->posX + MARTIAN_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if(checkMartianCollision(pMartian, &newX, &newY)) validMove = false;
            else validMove = true;
            break;

        case UP:
            newX = pMartian->posX;
            newY = pMartian->posY - MARTIAN_SPEED;
            relX0 = newX/TILE_SIZE;
            relY0 = newY/TILE_SIZE;
            relX1 = (newX+MARTIAN_SIZE-1)/TILE_SIZE;
            if(_mazeTiles[relX0][relY0].type == WALL || _mazeTiles[relX1][relY0].type == WALL) {
                if(pMartian->posY != (relY0*TILE_SIZE)+TILE_SIZE){
                    newY = pMartian->posY - (pMartian->posY - ((relY0*TILE_SIZE) + TILE_SIZE));
                    validMove = true;
                }
                else  validMove = false;
            }
            else if((newY+_mazeBounds.y0) < _mazeBounds.y0){
                if(pMartian->posY != (relY0*TILE_SIZE)){
                    newY = pMartian->posY - (pMartian->posY - (relY0*TILE_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if(checkMartianCollision(pMartian, &newX, &newY)) validMove = false;
            else validMove = true;
            break;

        case DOWN:
            newX = pMartian->posX;
            newY = pMartian->posY + MARTIAN_SPEED;
            relX0 = newX/TILE_SIZE;
            relY1 = (newY + MARTIAN_SIZE)/TILE_SIZE;
            relX1 = (newX+MARTIAN_SIZE-1)/TILE_SIZE;
            if(_mazeTiles[relX0][relY1].type == WALL || _mazeTiles[relX1][relY1].type == WALL) {
                if((pMartian->posY + MARTIAN_SIZE) != relY1*TILE_SIZE){
                    newY = pMartian->posY + ((relY1*TILE_SIZE) - (pMartian->posY + MARTIAN_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if((newY+MARTIAN_SIZE+_mazeBounds.y0) > _mazeBounds.y1){
                if((pMartian->posY + MARTIAN_SIZE) != relY1*TILE_SIZE){
                    newY = pMartian->posY + (((relY1*TILE_SIZE)+TILE_SIZE) - (pMartian->posY + MARTIAN_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if(checkMartianCollision(pMartian, &newX, &newY)) validMove = false;
            else validMove = true;
            break;
    }

    if(validMove){
        pMartian->posX = newX;
        pMartian->posY = newY;
    }

    return validMove;
}

static void *moveMartian(void *pMartianData){
    Martian *martian = (Martian*)pMartianData;
    martian->counter = 1;
    int prevResult = 0;
    int result = 0;
    // int prevSecTimer = _secTimer;
    // int readyCounter = 0;
    while(martian->running){
        // pthread_mutex_lock(&_mutex);
        // while(!martian->update && !martian->doWork) pthread_cond_wait(&martian->cond, &_mutex);

        if(martian->doWork){
            pthread_mutex_lock(&_mutex);
            martian->ready = 0;
            martian->doWork = 0;
            // printf("martian %s at counter = %d\n", martian->title, martian->counter);
            if(martian->currentEnergy == martian->maxEnergy && martian->counter == 1){
                martian->executed = 0; //For EDF
                printf("Start of %s at sec=%d\n", martian->title, _secTimer);
            }

            if(martian->counter == REFRESH_RATE ) {
                martian->currentEnergy--;
                martian->counter = 1;
                if(martian->currentEnergy == 0){
                    martian->executed = 1; //For EDF
                    printf("------ %s finished at sec=%d\n", martian->title, _secTimer);
                    pthread_mutex_unlock(&_mutex);
                    continue;
                }
                else{
                    if(!checkMove(martian)){
                        martian->direction = (rand() % (3 - 0 + 1)) + 0;
                    }
                    pthread_mutex_unlock(&_mutex);
                    continue;
                }
            }

            if(!checkMove(martian)){
                martian->direction = (rand() % (3 - 0 + 1)) + 0;
            }

            martian->counter++;
            // printf("Executing %s \t energy=%d \t counter=%d \t sec=%d\n", martian->title, martian->currentEnergy, martian->counter, _secTimer);
            pthread_mutex_unlock(&_mutex);
        }

        if(martian->update) {
            if((result = _secTimer%martian->period) == 0){
                if(result != prevResult){
                    martian->ready = 1;
                    martian->periodCounter++;
                    // printf("Timer of %s\n", martian->title);
                }
            }
          // prevSecTimer = _secTimer;
          prevResult = result;
          martian->update = 0;
      }

        // pthread_mutex_unlock(&_mutex);
        usleep(10); //Limit thread execution
    }
}

static void setup(){
    checkInit(al_init(), "Allegro");
    checkInit(al_install_keyboard(), "Keyboard");
    checkInit(al_install_mouse(), "Mouse");

    checkInit(al_init_image_addon(), "Image Addon");
    checkInit(al_init_primitives_addon(), "Primitives addon");

    _timer = al_create_timer(1.0/REFRESH_RATE);
    checkInit(_timer, "Timer");

    _eventQueue = al_create_event_queue();
    checkInit(_eventQueue, "Event Queue");

    _font = al_create_builtin_font();
    checkInit(_font, "Font");

    _display = al_create_display(SCREEN_WIDHT, SCREEN_HEIGHT);
    checkInit(_display, "Display");
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    al_register_event_source(_eventQueue, al_get_keyboard_event_source());
    al_register_event_source(_eventQueue, al_get_display_event_source(_display));
    al_register_event_source(_eventQueue, al_get_mouse_event_source());
    al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));

    _NKfont = nk_allegro5_font_create_from_file("./res/Roboto-Regular.ttf", 12, 0);
    _NKcontext = nk_allegro5_init(_NKfont, _display, SCREEN_WIDHT, SCREEN_HEIGHT);
    setCustomStyle(_NKcontext);

    _render = true;
    _exitLoop = false;
    martianAmount = 2;
    _secTimer = 0;
    _ticks = 0;
    _options._showHUD = 1;
    _options._showMartians = 1;
    _options._showMartianPos = 0;
    _options._errorPopUp = 0;
    _options._schedulingAlgorithm = EDF;

    checkInit(arrayInit(&_martians, 2, sizeof(Martian)), "Array of martians");
    checkInit(arrayInit(&_threads, 2, sizeof(pthread_t)), "Array of pthreads");
    checkInit(arrayInit(&_HUDfunctions, 2, sizeof(funcPtr)), "Array HUD functions");
    pthread_mutex_init(&_mutex, NULL);
}

static void loadAssets(){
    _mazeImg = al_load_bitmap("./res/maze.png");
    checkInit(_mazeImg, "Maze image");
    _mazeBounds.x0 = (SCREEN_WIDHT/2) - (al_get_bitmap_width(_mazeImg)/2);
    _mazeBounds.y0 = SCREEN_HEIGHT - al_get_bitmap_height(_mazeImg);
    _mazeBounds.x1 = (SCREEN_WIDHT/2) + (al_get_bitmap_width(_mazeImg)/2);
    _mazeBounds.y1 = SCREEN_HEIGHT;

    _mazeImgTiny = al_load_bitmap("./res/mazeTiny.png");
    checkInit(_mazeImgTiny, "Maze image tiny");
}

static void createMartians(){
    bool row = true;
    int initPosX, initPosY;
    int spacing = MARTIAN_SIZE + 5;
    int gap = spacing;

    int ener[] = {3,4}; //TEST
    int per[] = {6,9};  //TEST
    for(int i=0; i<martianAmount; i++){
        if(row){
            initPosX = (MAZE_START_X*TILE_SIZE) - gap;
            initPosY = MAZE_START_Y*TILE_SIZE;
        }
        else{
            initPosX = (MAZE_START_X*TILE_SIZE) - gap;
            initPosY = (MAZE_START_Y*TILE_SIZE) + (TILE_SIZE-MARTIAN_SIZE);
            gap += spacing;
        }
        Martian *martian = newMartian(initPosX, initPosY, RIGHT, ener[i], per[i]);
        martian->id = i;
        sprintf(martian->title, "%s %d","Martian", i);
        pthread_cond_init(&martian->cond, NULL);
        arrayInsert(&_martians, (void*)martian);

        arrayInsert(&_HUDfunctions, &martianHUD);
        row = !row;
    }
}

static void cleanUp(){
    nk_allegro5_font_del(_NKfont);
    nk_allegro5_shutdown();
    al_destroy_bitmap(_mazeImg);
    al_destroy_bitmap(_mazeImgTiny);
    al_destroy_font(_font);
    al_destroy_display(_display);
    al_destroy_timer(_timer);
    al_destroy_event_queue(_eventQueue);

    arrayFree(&_martians, true);
    arrayFree(&_threads, true);
    arrayFree(&_HUDfunctions, false);
}


void simLoop(){
    ALLEGRO_EVENT event;
    bool newEvent;
    Martian *nextMartian;
    int stopped = 0;
    char *message = "NO COLLISION";

    //Allocate and start the threads
    for(int i=0; i<martianAmount; i++){
        pthread_t *pthread = malloc(sizeof(pthread_t));
        arrayInsert(&_threads, (void*)pthread);
        pthread_create(pthread, NULL, moveMartian, _martians.array[i]);
    }


    int nextMartianIdx = 0;
    int scheduleError = 0;
    int currentState = READY;
    int wait = 0;
    int executeSchedule = 1;

    //Obtain first martian to execute
    // pthread_mutex_lock(&_mutex);
    switch(_options._schedulingAlgorithm){
        case RM:
            rm_shchedule(&currentState, &nextMartianIdx, &wait);
            break;
        case EDF:
            edf_schedule(&currentState, &nextMartianIdx, &wait, _secTimer);
            break;
    }
    // pthread_mutex_unlock(&_mutex);

    al_start_timer(_timer);
    while(1){
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 1.0/10.0);
        newEvent = al_wait_for_event_until(_eventQueue, &event, &timeout);
        // al_wait_for_event(_eventQueue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                // printf("FRAME COUNTER=%d\n", _ticks);
                if(!scheduleError){
                    if(_ticks == REFRESH_RATE-1){// 1 second
                        _secTimer++;
                        // printf("Second %d\n", _secTimer);
                        _ticks = -1;
                    }
                }

                if(executeSchedule){
                    //Next thread acquires lock and executes
                    if(!wait){
                        nextMartian = ((Martian*)_martians.array[nextMartianIdx]);
                        // printf("%s period Counter = %d ------\n", nextMartian->title, nextMartian->periodCounter);
                        switch(currentState){
                            case 0: //Ready
                                nextMartian->currentEnergy = nextMartian->maxEnergy;
                                nextMartian->doWork = 1;
                                // pthread_cond_signal(&nextMartian->cond);
                                // printf("Start of %s at second %d\n", nextMartian->title, _secTimer);
                                break;
                            case 1: //Executing
                                nextMartian->doWork = 1;
                                // pthread_cond_signal(&nextMartian->cond);
                                // printf("Continue executing %s with energy=%d at second %d\n", nextMartian->title, nextMartian->currentEnergy, _secTimer);
                                break;
                        }
                    }

                    //Signal all threads to update their time counter
                    for(int i=0; i<martianAmount; i++){
                        ((Martian*)_martians.array[i])->update = 1;
                        while(((Martian*)_martians.array[i])->update == 1); //Wait until thread is updated
                        // pthread_cond_signal(&((Martian*)_martians.array[i])->cond);

                    }

                    //Acquire lock and determine the next thread to execute
                    pthread_mutex_lock(&_mutex);
                    scheduleError = checkSchedulingError(_options._schedulingAlgorithm);
                    if(scheduleError){
                        executeSchedule = 0;
                        _options._errorPopUp = 1;
                        stopAllThreads();
                        printf("Scheduling error!!!\n");
                    }
                    // pthread_mutex_unlock(&_mutex);

                    // pthread_mutex_lock(&_mutex);
                    switch(_options._schedulingAlgorithm){
                        case RM:
                            rm_shchedule(&currentState, &nextMartianIdx, &wait);
                            break;
                        case EDF:
                            edf_schedule(&currentState, &nextMartianIdx, &wait, _secTimer);
                            break;
                    }
                    pthread_mutex_unlock(&_mutex);
                }
                _ticks++;
                _render = true;
                // printf("#########################################\n");
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    stopAllThreads();
                    _exitLoop = true;
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                stopAllThreads();
                _exitLoop = true;
                break;
        } //End event switch

        if(_exitLoop) break;

        //Nuklear event handling
        nk_input_begin(_NKcontext);
        if (newEvent) {
            while (newEvent) {
                nk_allegro5_handle_event(&event);
                newEvent = al_get_next_event(_eventQueue, &event);
            }
        }
        nk_input_end(_NKcontext);

        //Draw Nuklear UI
        drawMenu(_NKcontext, _mazeBounds);
        if(_options._showHUD)
            drawMartianHUD(_NKcontext, _mazeBounds, &_martians, &_HUDfunctions);
        if(_options._errorPopUp)
            errorPopUp(_NKcontext, _secTimer);

        //Render
        if(_render && al_is_event_queue_empty(_eventQueue)){
            al_clear_to_color(al_map_rgb(19, 43, 81));
            al_draw_text(_font, al_map_rgb(0, 0, 0), 0, 0, 0, message);
            al_draw_bitmap(_mazeImg, _mazeBounds.x0, _mazeBounds.y0, 0);

            //Draw the martians
            if(_options._showMartians){
                for(int i=0; i<martianAmount; i++){
                    Martian *martian = (Martian*)_martians.array[i];
                    al_draw_filled_rectangle(martian->posX + _mazeBounds.x0,
                        martian->posY + _mazeBounds.y0,
                        martian->posX + _mazeBounds.x0 + MARTIAN_SIZE,
                        martian->posY + _mazeBounds.y0 + MARTIAN_SIZE,
                        al_map_rgb(0, 255, 0));
                    char tmp[4];
                    sprintf(tmp, "%d", i);
                    al_draw_text(_font, al_map_rgb(0, 0, 0),
                        martian->posX + _mazeBounds.x0,
                        martian->posY + _mazeBounds.y0,
                        0, tmp);
                }
            }

            nk_allegro5_render();
            al_flip_display();
            _render = false;
        }

        else{ //Nuklear needs to be render all time
            al_clear_to_color(al_map_rgb(19, 43, 81));
            nk_allegro5_render();
        }

    } //End Loop

    //Stop all threads
    for(int i=0; i<martianAmount; i++)
        pthread_join(*(pthread_t*)_threads.array[i], NULL);

    cleanUp();
}


int main(){
    srand(time(0));
    setup();
    loadAssets();
    loadMazeTiles(_mazeTiles, _mazeImgTiny);
    createMartians();
    simLoop();

    exit(0);
}
