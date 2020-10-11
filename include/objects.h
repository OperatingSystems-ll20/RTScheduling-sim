#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <allegro5/allegro5.h>
#include <consts.h>

enum TILE_TYPE {WALL, PATH};

typedef struct MazeTiles{
    int x;
    int y;
    int type;
} MazeTile;

MazeTile _mazeTiles[MAZE_WIDTH][MAZE_HEIGHT];

void loadMazeTiles(ALLEGRO_BITMAP* pMazeImg);



#endif
