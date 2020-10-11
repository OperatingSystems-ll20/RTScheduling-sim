#ifndef _COLLISION_H
#define _COLLISION_H

#include <allegro5/allegro5.h>

enum TILE_TYPE {WALL, PATH};

typedef struct MazeTiles{
    int x;
    int y;
    int type;
} MazeTile;

MazeTile _mazeTiles[480][480];

void loadMazeTiles(ALLEGRO_BITMAP* pMazeImg);



#endif
