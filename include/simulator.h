#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <allegro5/allegro5.h>
#include <pthread.h>
#include <dynamicArray.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear/nuklear.h>
#include <nuklear/nuklear_allegro5.h>

static const double REFRESH_RATE = 30.0;
bool _render;
bool _exitLoop;

int martianAmount;
unsigned int _secTimer;
unsigned int _ticks;


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

MazeBounds _mazeBounds;
MazeTile _mazeTiles[MAZE_WIDTH][MAZE_HEIGHT];
Options _options;

DynamicArray _martians;
DynamicArray _threads;
DynamicArray _HUDfunctions;

pthread_mutex_t _mutex;

static void checkInit(bool pTest, const char *pDescription);
static void stopAllThreads();
static bool checkMartianCollision(Martian *pMartian, int *pNewX, int *pNewY);
static bool checkMove(Martian *pMartian);
static void *moveMartian(void *pMartianData);

static void setup();
static void loadAssets();
static void createMartians();
static void cleanUp();
void simLoop();


#endif
