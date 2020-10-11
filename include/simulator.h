#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <allegro5/allegro5.h>

static const double REFRESH_RATE = 30.0;

extern bool _render;
extern bool _exitLoop;

ALLEGRO_DISPLAY *_display;
ALLEGRO_TIMER *_timer;
ALLEGRO_EVENT_QUEUE *_eventQueue;
ALLEGRO_FONT *_font;
ALLEGRO_BITMAP *_mazeImg;
ALLEGRO_BITMAP *_mazeImgTiny;

//Temporal
int mouseX, mouseY;
int squareX, squareY;

void checkInit(bool pTest, const char *pDescription);

void setup();
void loadAssets();
void cleanUp();
void simLoop();


#endif
