#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <allegro5/allegro5.h>
#include <pthread.h>
#include <consts.h>

enum TILE_TYPE {WALL=0, PATH};
enum DIRECTION {LEFT = 0, RIGHT, UP, DOWN};

typedef struct {
    int _showHUD;
    int _showMartians;
    int _showMartianPos;
} Options;

typedef struct MazeTiles{
    int x;
    int y;
    int type;
} MazeTile;

typedef struct {
    int x0;
    int y0;
    int x1;
    int y1;
} MazeBounds;

typedef struct Martians{
    int id;
    char title[15];
    int posX;
    int posY;
    int direction;
    int maxEnergy;
    size_t currentEnergy;
    int period;
    int ready;
    int running;
    int doWork;
    int counter;
    int update;
    pthread_cond_t cond;
} Martian;

void loadMazeTiles(MazeTile pMazeTiles[MAZE_WIDTH][MAZE_HEIGHT], ALLEGRO_BITMAP* pMazeImg);
Martian *newMartian(int pPosX, int pPosY, int pInitalDirection, int pMaxEnergy, int pPeriod);



#endif
