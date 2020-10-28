#include <consts.h>
#include <objects.h>

/**
 * @brief Fills an array of struct that holds the position of the tiles
 *        of the maze and also its type (Wall or Path)
 * 
 * @param pMazeTiles Struct of type MazeTyle
 * @param pMazeTiny  Allegro bitmap of the maze on scale 1 tyle->1px
 */
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


/**
 * @brief Allocates memory for a new struct of type Martian, which holds all the
 *        relevant information about one martian, such as its position, energy, period, etc.
 * 
 * @param pPosX             Initial X position
 * @param pPosY             Initial Y position
 * @param pInitalDirection  Initial direction
 * @param pMaxEnergy        Energy  
 * @param pPeriod           Period
 * @return Martian*         Pointer to the struct allocated
 */
Martian *newMartian(int pPosX, int pPosY, int pInitalDirection, int pMaxEnergy, int pPeriod){
    Martian *martian = malloc(sizeof(Martian));
    martian->posX = pPosX;
    martian->posY = pPosY;
    martian->martianState = NOT_DONE;
    martian->direction = pInitalDirection;
    martian->currentEnergy = 0;
    martian->maxEnergy = pMaxEnergy;
    martian->period = pPeriod;
    martian->ready = 1;
    martian->running = 1;
    martian->doWork = 0;
    martian->update = 0;
    martian->arrivalTime = 0;
    martian->executed = -1;
    martian->periodCounter = 1;
    martian->flagExec = 0;
    return martian;
}
