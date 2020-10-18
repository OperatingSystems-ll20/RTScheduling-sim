#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <allegro5/allegro5.h>
#include <pthread.h>
#include <dynamicArray.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_ALLEGRO5_IMPLEMENTATION
#include <nuklear.h>
#include <nuklear_allegro5.h>

static const double REFRESH_RATE = 30.0;

extern bool _render;
extern bool _exitLoop;

int martianAmount;

struct {
    int x0;
    int y0;
    int x1;
    int y1;
} MazeBounds;

/** ALLEGRO **/
ALLEGRO_DISPLAY *_display;
ALLEGRO_TIMER *_timer;
ALLEGRO_EVENT_QUEUE *_eventQueue;
ALLEGRO_FONT *_font;
ALLEGRO_BITMAP *_mazeImg;
ALLEGRO_BITMAP *_mazeImgTiny;

//** NUKLEAR **/
NkAllegro5Font *_NKfont;
struct nk_context *_NKcontext;

MazeTile _mazeTiles[MAZE_WIDTH][MAZE_HEIGHT];

DynamicArray _martians;
DynamicArray _threads;

pthread_mutex_t _mutex;

void checkInit(bool pTest, const char *pDescription);
bool checkMove(Martian *pMartian);

void setup();
void loadAssets();
void createMartians();
void cleanUp();
void simLoop();


#endif
