#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <consts.h>
#include <objects.h>
#include <UI.h>
#include <simulator.h>

bool _render;
bool _exitLoop;

static void checkInit(bool pTest, const char *pDescription){
    if(pTest) return;
    printf("Error: Couldn't initialize %s\n", pDescription);
    exit(1);
}

static void stopAllThreads(){
    for(int i=0; i<martianAmount; i++){
        ((Martian*)_martians.array[i])->running = 0;
        // ((Martian*)_martians.array[i])->doWork = 1;
        pthread_cond_signal(&((Martian*)_martians.array[i])->cond);
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
    int counter = 0;
    int prevResult = 0;
    int result = 0;
    while(martian->running){
        // pthread_mutex_lock(&_mutex);
        // while(!martian->doWork) pthread_cond_wait(&martian->cond, &_mutex);

        if(martian->doWork){
            martian->doWork = 0;
            martian->ready = 0;
            if(martian->currentEnergy != 0){
                if(counter == REFRESH_RATE) {
                    martian->currentEnergy--;
                    counter = 0;
                }
            }

            // if(martian->currentEnergy < 0) martian->currentEnergy = martian->maxEnergy;
            if(!checkMove(martian))
                martian->direction = (rand() % (3 - 0 + 1)) + 0;

            counter++;
        }
        if((result = _secTimer%martian->period) == 0){
            if(result != prevResult){
                martian->ready = 1;
                printf("Timer of martian %d\n", martian->id);
            }
        }
        prevResult = result;
        // pthread_mutex_unlock(&_mutex);
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
    initUI(&_options);
    setCustomStyle(_NKcontext);

    _render = true;
    _exitLoop = false;
    martianAmount = 2;
    _secTimer = 0;
    _prevMartian = _currentMartian = 0;
    _options._showHUD = 1;
    _options._showMartians = 1;
    _options._showMartianPos = 0;



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

    int ener[] = {1, 2}; //TEST
    int per[] = {6, 9};  //TEST
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

static int nextShortestPeriod(int pIndexToIgnore){
    int result = 0;
    int found = 0;
    for(int i=0; i<martianAmount; i++){
        Martian *martian = (Martian*)_martians.array[i];
        if(i == pIndexToIgnore && i == martianAmount-1)
            continue;
        else if(i == pIndexToIgnore){
            result++;
            continue;
        }

        if(martian->ready && martian->period < ((Martian*)_martians.array[result])->period) {
            found = 1;
            result = i;
        }
    }
    if(found) return result;
    else return -1;
}


void simLoop(){
    ALLEGRO_EVENT event;
    bool newEvent;
    al_start_timer(_timer);
    int frameCounter = 0;
    Martian *nextMartian;
    int stopped = 0;
    char *message = "NO COLLISION";

    //Allocate and start the threads
    for(int i=0; i<martianAmount; i++){
        pthread_t *pthread = malloc(sizeof(pthread_t));
        arrayInsert(&_threads, (void*)pthread);
        pthread_create(pthread, NULL, moveMartian, _martians.array[i]);
    }

    while(1){
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 1.0/10.0);
        newEvent = al_wait_for_event_until(_eventQueue, &event, &timeout);
        // al_wait_for_event(_eventQueue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                //Select the index of the martian with shortest period
                for(int i=0; i<martianAmount; i++){
                    nextMartian = (Martian*)_martians.array[i];
                    if(nextMartian->period < ((Martian*)_martians.array[_currentMartian])->period )
                        _currentMartian = i;
                    // pthread_cond_signal(&((Martian*)_martians.array[i])->cond);
                }
                nextMartian = (Martian*)_martians.array[_currentMartian]; //Martian with shortest period

                // if(nextMartian->ready == 0 && nextMartian->currentEnergy == 0){
                //     int index = nextShortestPeriod(_currentMartian);
                //     if(index >= 0) nextMartian = (Martian*)_martians.array[index];
                // }

                // //Check for martians that are ready to execute
                // for(int i=0; i<martianAmount; i++){
                //     Martian *martian = (Martian*)_martians.array[i];
                //     int index = 0;
                //     if(martian->id == nextMartian->id){
                //         if(martian->ready) break;
                //         else if(martian->currentEnergy == 0){
                //             index = nextShortestPeriod(i);
                //             if(index >= 0) _currentMartian = index;
                //             break;
                //         }
                //         else break;
                //     }
                //     else{
                //         if(martian->ready){
                //             //Shortest period martian needs exection
                //             if( (nextMartian->ready && nextMartian->currentEnergy == 0) ||
                //                 (!nextMartian->ready && nextMartian->currentEnergy != 0) )
                //                 break;
                //             else{
                //                 index = nextShortestPeriod(_currentMartian);
                //                 if(index >= 0) _currentMartian = index;
                //                 break;
                //             }
                //         }
                //     }
                // }


                //Scheduling error!!!!
                if(nextMartian->currentEnergy != 0 && nextMartian->ready == 1){
                    if(!stopped) {
                        printf("Scheduling error!!!!\n");
                        stopAllThreads();
                        stopped = 1;
                    }
                }

                //Start new execution period
                else if(nextMartian->ready){
                    nextMartian->currentEnergy = nextMartian->maxEnergy;
                    nextMartian->doWork = 1;
                }

                //Keep executing
                else{
                    if(nextMartian->currentEnergy != 0)
                        nextMartian->doWork = 1;
                }

                // _prevMartian = _currentMartian;


                if(frameCounter == REFRESH_RATE){// 1 second
                    _secTimer++;
                    //printf("%d%%%d = %d\n", _secTimer, ((Martian*)_martians.array[0])->period, _secTimer%((Martian*)_martians.array[0])->period);
                    frameCounter = 0;
                }
                frameCounter++;
                _render = true;
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
