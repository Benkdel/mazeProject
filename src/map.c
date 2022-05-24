#include "headers.h"

/**
 * setOuterWalls - 
 * 
 * 
 * 
 */
void setOuterWalls(sdl_window *w, outer_walls *walls, double size)
{

    double frame = (1 - size) / 2;

    // store map values
    walls->x = w->SCREEN_WIDTH * frame;
    walls->y = w->SCREEN_HEIGHT * frame;
    walls->w = w->SCREEN_WIDTH * (1 - frame) - walls->x;
    walls->h = w->SCREEN_HEIGHT * (1 - frame) - walls->y;

    // set SDL rect points
    walls->sdl_walls.x = walls->x;
    walls->sdl_walls.y = walls->y;
    walls->sdl_walls.w = walls->w;
    walls->sdl_walls.h = walls->h;

    printf("Map Data: \n");
    printf("Screen Width: %d, Screen Height: %d\n", w->SCREEN_WIDTH, w->SCREEN_HEIGHT);
    printf("x: %d, y: %d, w: %d, h: %d\n", walls->x, walls->y, walls->w, walls->h);
}


/**
 * renderOuterWalls - 
 * 
 * 
 */
void renderOuterWalls(sdl_window *w, outer_walls *walls)
{
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 255, 1);
    SDL_RenderDrawRect(w->renderer, &walls->sdl_walls);
}

void renderWall(sdl_window *w, line *l)
{
    SDL_SetRenderDrawColor(w->renderer, 255, 0, 0, 1);
    SDL_RenderDrawLine(w->renderer, l->x1, l->y1, l->x2, l->y2);
}
