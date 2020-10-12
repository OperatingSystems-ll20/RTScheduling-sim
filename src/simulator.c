#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <consts.h>
#include <objects.h>
#include <simulator.h>

bool _render;
bool _exitLoop;

void checkInit(bool pTest, const char *pDescription){
    if(pTest) return;
    printf("Error: Couldn't initialize %s\n", pDescription);
    exit(1);
}

bool checkMove(Martian *pMartian){
    int x1 = pMartian->posX + MARTIAN_SIZE;
    int y1 = pMartian->posY + MARTIAN_SIZE;
    int newX, newY;
    int relX0, relY0, relX1, relY1;
    bool validMove;

    switch(pMartian->direction){
        case LEFT:
            newX = pMartian->posX - MARTIAN_SPEED;
            newY = pMartian->posY;
            relX0 = newX/TILE_SIZE;
            relY0 = newY/TILE_SIZE;
            if(_mazeTiles[relX0][relY0].type == WALL) {
                if(pMartian->posX != (relX0*TILE_SIZE)+TILE_SIZE){
                    newX = pMartian->posX - (pMartian->posX - ((relX0*TILE_SIZE) + TILE_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if((newX + MazeBounds.x0) < MazeBounds.x0){
                validMove = false;
            }
            else validMove = true;
            break;

        case RIGHT:
            newX = pMartian->posX + MARTIAN_SPEED;
            newY = pMartian->posY;
            relX1 = (newX + MARTIAN_SIZE)/TILE_SIZE;
            relY0 = newY/TILE_SIZE;
            if(_mazeTiles[relX1][relY0].type == WALL ) {
                if((pMartian->posX + MARTIAN_SIZE) != (relX1*TILE_SIZE)){
                    newX = pMartian->posX + ((relX1*TILE_SIZE) - (pMartian->posX + MARTIAN_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if((newX+MARTIAN_SIZE+MazeBounds.x0) > MazeBounds.x1)
                validMove = false;
            else validMove = true;
            break;

        case UP:
            newX = pMartian->posX;
            newY = pMartian->posY - MARTIAN_SPEED;
            relX0 = newX/TILE_SIZE;
            relY0 = newY/TILE_SIZE;
            if(_mazeTiles[relX0][relY0].type == WALL) {
                if(pMartian->posY != (relY0*TILE_SIZE)+TILE_SIZE){
                    newY = pMartian->posY - (pMartian->posY - ((relY0*TILE_SIZE) + TILE_SIZE));
                    validMove = true;
                }
                else  validMove = false;
            }
            else if((newY+MazeBounds.y0) < MazeBounds.y0)
                validMove = false;
            else validMove = true;
            break;

        case DOWN:
            newX = pMartian->posX;
            newY = pMartian->posY + MARTIAN_SPEED;
            relX0 = newX/TILE_SIZE;
            relY1 = (newY + MARTIAN_SIZE)/TILE_SIZE;
            if(_mazeTiles[relX0][relY1].type == WALL) {
                if((pMartian->posY + MARTIAN_SIZE) != relY1*TILE_SIZE){
                    newY = pMartian->posY + ((relY1*TILE_SIZE) - (pMartian->posY + MARTIAN_SIZE));
                    validMove = true;
                }
                else validMove = false;
            }
            else if((newY+MARTIAN_SIZE+MazeBounds.y0) > MazeBounds.y1)
                validMove = false;
            else validMove = true;
            break;
    }

    if(validMove){
        pMartian->posX = newX;
        pMartian->posY = newY;
    }

    return validMove;
}

void setup(){
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

    al_register_event_source(_eventQueue, al_get_keyboard_event_source());
    al_register_event_source(_eventQueue, al_get_display_event_source(_display));
    al_register_event_source(_eventQueue, al_get_mouse_event_source());
    al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));

    _render = true;
    _exitLoop = false;
    squareX = squareY = 0;

    // al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    // al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    // al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
}


void loadAssets(){
    _mazeImg = al_load_bitmap("./res/maze.png");
    checkInit(_mazeImg, "Maze image");
    MazeBounds.x0 = (SCREEN_WIDHT/2) - (al_get_bitmap_width(_mazeImg)/2);
    MazeBounds.y0 = SCREEN_HEIGHT - al_get_bitmap_height(_mazeImg);
    MazeBounds.x1 = (SCREEN_WIDHT/2) + (al_get_bitmap_width(_mazeImg)/2);
    MazeBounds.y1 = SCREEN_HEIGHT;

    _mazeImgTiny = al_load_bitmap("./res/mazeTiny.png");
    checkInit(_mazeImgTiny, "Maze image tiny");
}

void createMartians(){
    _testMartian = newMartian(MAZE_START_X*TILE_SIZE, MAZE_START_Y*TILE_SIZE, RIGHT, 5, 10);
}


void cleanUp(){
    al_destroy_bitmap(_mazeImg);
    al_destroy_bitmap(_mazeImgTiny);
    al_destroy_font(_font);
    al_destroy_display(_display);
    al_destroy_timer(_timer);
    al_destroy_event_queue(_eventQueue);
}


void simLoop(){
    ALLEGRO_EVENT event;
    al_start_timer(_timer);
    char *message = "NO COLLISION";

    while(1){
        al_wait_for_event(_eventQueue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                if(checkMove(_testMartian)){
                    message = "MOVING MARTIAN";
                }
                else{ //GET NEW DIRECTION
                    _testMartian->direction = (rand() % (3 - 0 + 1)) + 0;
                    printf("New direction is = %d\n", _testMartian->direction);
                    message = "NEW DIRECTION";
                    break;
                }
                _render = true;
                break;


                // if( (squareX+16) > MazeBounds.x0 && squareX < MazeBounds.x1 &&
                //     (squareY+16) > MazeBounds.y0 && squareY < MazeBounds.y1 ){
                //
                //     int relX0 = (squareX-MazeBounds.x0)/TILE_SIZE;
                //     int relY0 = (squareY-MazeBounds.y0)/TILE_SIZE;
                //     int relX1 = ((squareX+15)-MazeBounds.x0)/TILE_SIZE;
                //     int relY1 = ((squareY+15)-MazeBounds.y0)/TILE_SIZE;
                //
                //     if (_mazeTiles[relX0][relY0].type == WALL ||
                //         _mazeTiles[relX1][relY0].type == WALL ||
                //         _mazeTiles[relX0][relY1].type == WALL ||
                //         _mazeTiles[relX1][relY1].type == WALL){
                //             message = "COLLISION WITH WALL";
                //     }
                //     else{
                //         message = "OVER PATH!";
                //     }
                //
                // }
                // else
                //     message = "NO COLLISION";



            case ALLEGRO_EVENT_MOUSE_AXES:
                mouseX = event.mouse.x;
                mouseY = event.mouse.y;
                squareX = mouseX-8;
                squareY = mouseY-8;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    _exitLoop = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                _exitLoop = true;
                break;
        }

        if(_exitLoop) break;

        if(_render && al_is_event_queue_empty(_eventQueue)){
            al_clear_to_color(al_map_rgb(255, 255, 255));
            al_draw_text(_font, al_map_rgb(0, 0, 0), 0, 0, 0, message);
            al_draw_bitmap(_mazeImg, MazeBounds.x0, MazeBounds.y0, 0);
            al_draw_filled_rectangle(squareX, squareY, squareX + 16, squareY + 16, al_map_rgb(255, 0, 0));

            //Test martian
            al_draw_filled_rectangle(_testMartian->posX + MazeBounds.x0,
                _testMartian->posY + MazeBounds.y0,
                _testMartian->posX + MazeBounds.x0 + MARTIAN_SIZE,
                _testMartian->posY + MazeBounds.y0 + 16,
                al_map_rgb(0, 255, 0));

            al_flip_display();
            _render = false;
        }

    }

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
