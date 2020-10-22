#include <consts.h>
#include <objects.h>


void loadMazeTiles(MazeTile pMazeTiles[MAZE_WIDTH][MAZE_HEIGHT], ALLEGRO_BITMAP* pMazeTiny){
    int width = al_get_bitmap_width(pMazeTiny);
    int height = al_get_bitmap_height(pMazeTiny);
    al_lock_bitmap(pMazeTiny, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);

    ALLEGRO_COLOR pixel;
    unsigned char r, g, b;
    for(int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            MazeTile tile;
            tile.x = ((SCREEN_WIDHT/2) - (MAZE_WIDTH/2)) + TILE_SIZE * i;
            tile.y = (SCREEN_HEIGHT - MAZE_HEIGHT) + TILE_SIZE * j;
            pixel = al_get_pixel(pMazeTiny, i, j);
            al_unmap_rgb(pixel, &r, &g, &b);
            tile.type = (r == 0xff) ? PATH : WALL;
            pMazeTiles[i][j] = tile;
        }
    }
    al_unlock_bitmap(pMazeTiny);
}


Martian *newMartian(int pPosX, int pPosY, int pInitalDirection, int pMaxEnergy, int pPeriod){
    Martian *martian = malloc(sizeof(Martian));
    martian->posX = pPosX;
    martian->posY = pPosY;
    martian->direction = pInitalDirection;
    martian->currentEnergy = 0;
    martian->maxEnergy = pMaxEnergy;
    martian->period = pPeriod;
    martian->ready = 1;
    martian->running = 1;
    martian->doWork = 0;
    martian->update = 0;
    martian->executed = -1;
    martian->periodCounter = 1;
    return martian;
}
