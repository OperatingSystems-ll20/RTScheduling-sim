#include <consts.h>
#include <collision.h>


void loadMazeTiles(ALLEGRO_BITMAP* pMazeTiny){
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
            _mazeTiles[i][j] = tile;
        }
    }
    al_unlock_bitmap(pMazeTiny);
}
