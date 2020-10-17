#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <allegro5/allegro5.h>
#include <pthread.h>

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

ALLEGRO_DISPLAY *_display;
ALLEGRO_TIMER *_timer;
ALLEGRO_EVENT_QUEUE *_eventQueue;
ALLEGRO_FONT *_font;
ALLEGRO_BITMAP *_mazeImg;
ALLEGRO_BITMAP *_mazeImgTiny;

MazeTile _mazeTiles[MAZE_WIDTH][MAZE_HEIGHT];

//TEST MARTIAN
Martian **_martians;
pthread_t *_threads;
pthread_mutex_t _mutex;


//Temporal
int mouseX, mouseY;
int squareX, squareY;

void checkInit(bool pTest, const char *pDescription);
bool checkMove(Martian *pMartian);

void setup();
void loadAssets();
void createMartians();
void cleanUp();
void simLoop();


#endif
