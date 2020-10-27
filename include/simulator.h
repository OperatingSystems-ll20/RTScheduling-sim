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

static bool _render;

int _martianAmount;
int _outsideCounter;
unsigned int _secTimer;
unsigned int _ticks;
int _scheduleError;


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
struct nk_image *_reportImg;

MazeBounds _mazeBounds;
MazeTile _mazeTiles[MAZE_WIDTH][MAZE_HEIGHT];
Options _options;

DynamicArray _martians;
DynamicArray _martianColors;
DynamicArray _threads;
DynamicArray _HUDfunctions;

pthread_mutex_t _mutex;

static void checkInit(bool pTest, const char *pDescription);
static bool checkMartianCollision(Martian *pMartian, int *pNewX, int *pNewY);
static bool checkMove(Martian *pMartian);
static void *moveMartian(void *pMartianData);

static void setDefaultOptions();
static void setup();
static void loadAssets();
static ALLEGRO_COLOR* getRandomColor();
static void createReportImage();
static void insertMartian();
static void createMartiansAutomaticMode();
static void startThreads();
static void stopAllThreads();
static void cleanUp();
static void checkUIEvents();
static void render();
void simLoop();


#endif
