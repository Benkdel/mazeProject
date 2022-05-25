#include "headers.h"

/**
 * setOuterWalls -
 *
 *
 *
 */
void setOuterWalls(outer_walls *walls, SDL_Rect *port, double size)
{
    double frame = (1 - size) / 2;

    // store map values
    walls->x = port->w * frame;
    walls->y = port->h * frame;
    walls->w = port->w * (1 - frame) - walls->x;
    walls->h = port->h * (1 - frame) - walls->y;

    // set SDL rect points
    walls->sdl_walls.x = walls->x;
    walls->sdl_walls.y = walls->y;
    walls->sdl_walls.w = walls->w;
    walls->sdl_walls.h = walls->h;

    printf("Map Data: \n");
    printf("Screen Width: %d, Screen Height: %d\n", port->w, port->h);
    printf("x: %d, y: %d, w: %d, h: %d\n", walls->x, walls->y, walls->w, walls->h);
}

/**
 * renderOuterWalls -
 *
 *
 */
void renderOuterWalls(const sdl_window *w, const outer_walls *walls, const SDL_Rect *port)
{
    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 255, 1);
    SDL_RenderDrawRect(w->renderer, &walls->sdl_walls);
}

/**
 * setInnerWalls
 *
 *
 */
void setInnerWalls(inner_walls *iWalls, const outer_walls *oWalls)
{
    unsigned int i;

    for (i = 0; i < MAX_WALLS; i++)
    {
        iWalls->l[i].x1 = (rand() % (oWalls->w - oWalls->x)) + oWalls->x;
        iWalls->l[i].x2 = (rand() % (oWalls->w - oWalls->x)) + oWalls->x;

        iWalls->l[i].y1 = (rand() % (oWalls->h - oWalls->y)) + oWalls->y;
        iWalls->l[i].y2 = (rand() % (oWalls->h - oWalls->y)) + oWalls->y;
    }
}

/**
 * renderInnerWalls -
 *
 *
 */
void renderInnerWalls(const sdl_window *w, const inner_walls *iWalls, const SDL_Rect *port)
{
    unsigned int i;

    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 255, 0, 0, 1);

    for (i = 0; i < MAX_WALLS; i++)
    {
        SDL_RenderDrawLine(w->renderer, iWalls->l[i].x1, iWalls->l[i].y1, iWalls->l[i].x2, iWalls->l[i].y2);
    }
}

/**
 * setMinimapPort -
 *
 *
 */
void setMinimapPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H)
{
    vp->x = 0;
    vp->y = 0;
    vp->w = scr_W / 2;
    vp->h = scr_H;
}

/**
 * setWorldPort -
 *
 *
 */
void setWorldPort(SDL_Rect *vp, unsigned int scr_W, unsigned int scr_H)
{
    vp->x = scr_W / 2;
    vp->y = 0;
    vp->w = scr_W;
    vp->h = scr_H;
}
