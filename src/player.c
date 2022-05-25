#include "headers.h"

/**
 * playerInit - 
 * 
 * 
 */
void playerInit(sdl_window *w, player *p, vec3 pos, vec3 dir, float fov)
{
    p->pos = pos;
    p->dir = p->dir;

    //textureInit(&p->text);
    //textureLoad(&p->text, w, "../assets/images/flash_light.png");
}

/**
 * updatePlayerDir - 
 * 
 * 
 */
void playerUpdateDir(player *p, vec3 dir)
{
    p->dir = dir;
}

/**
 * playerUpdatePos - 
 * 
 */
void playerUpdatePos(player *p, vec3 pos)
{
    float velocity = 1.05f;
    p->pos = scalarMult(pos, velocity);
}

/**
 * renderPlayer -
 * 
 */
void playerRender(sdl_window *w, player *p, SDL_Rect *port)
{
    int i = 0;

    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 51, 1);
    drawCircle(w->renderer, (int32_t)p->pos.x, (int32_t)p->pos.y, CONST_PI * 2);

    /* render texture as player */

    p->noRays = MAX_RAYS;

    for (i = 0; i < p->noRays; i++)
    {
        p->rays[i].pos = p->pos;
        p->rays[i].dir = vectorAddition(p->dir, getVecFromAngle(100, i));
    }
}

