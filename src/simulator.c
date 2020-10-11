#include <stdio.h>
#include <allegro5/allegro_image.h>
#include <simulator.h>

bool _render;
bool _exitLoop;

void checkInit(bool pTest, const char *pDescription){
    if(pTest) return;
    printf("Error: Couldn't initialize %s\n", pDescription);
    exit(1);
}

void setup(){
    checkInit(al_init(), "Allegro");
    checkInit(al_install_keyboard(), "Keyboard");

    _timer = al_create_timer(1.0/REFRESH_RATE);
    checkInit(_timer, "Timer");

    _eventQueue = al_create_event_queue();
    checkInit(_eventQueue, "Event Queue");

    checkInit(al_init_image_addon(), "Image Addon");

    _display = al_create_display(SCREEN_WIDHT, SCREEN_HEIGHT);
    checkInit(_display, "Display");

    al_register_event_source(_eventQueue, al_get_keyboard_event_source());
    al_register_event_source(_eventQueue, al_get_display_event_source(_display));
    al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));

    _render = true;
    _exitLoop = false;

    // al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    // al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    // al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
}


void loadAssets(){
    _mazeImg = al_load_bitmap("./res/maze.png");
    checkInit(_mazeImg, "Maze image");
}


void cleanUp(){
    al_destroy_bitmap(_mazeImg);
    al_destroy_display(_display);
    al_destroy_timer(_timer);
    al_destroy_event_queue(_eventQueue);
}


void simLoop(){
    ALLEGRO_EVENT event;
    al_start_timer(_timer);

    while(1){
        al_wait_for_event(_eventQueue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                _render = true;
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
            al_draw_bitmap(_mazeImg, (SCREEN_WIDHT/2)-240, ((SCREEN_HEIGHT/2)-240)*2, 0);
            al_flip_display();
            _render = false;
        }

    }

    cleanUp();
}


int main(){
    setup();
    loadAssets();
    simLoop();

    exit(0);
}
