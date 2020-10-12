#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <allegro5/allegro5.h>
#include <consts.h>

enum TILE_TYPE {WALL=0, PATH};
enum DIRECTION {LEFT = 0, RIGHT, UP, DOWN};

typedef struct MazeTiles{
    int x;
    int y;
    int type;
} MazeTile;

typedef struct Martians{
    int posX;
    int posY;
    int direction;
    int energy;
    int period;
} Martian;

void loadMazeTiles(MazeTile pMazeTiles[MAZE_WIDTH][MAZE_HEIGHT], ALLEGRO_BITMAP* pMazeImg);
Martian *newMartian(int pPosX, int pPosY, int pInitalDirection, int pEnergy, int pPeriod);



#endif
