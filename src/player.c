#include "headers.h"

/**
 * playerInit
 * 
 * 
 */
void playerInit(player *p, vec3 pos, vec3 dir, float fov)
{
    p->pos = pos;
    normalizeVec(&dir, pos);
    p->dir = dir;

    p->fov = fov;
}

/**
 * updatePlayerDir
 * 
 * 
 */
void playerUpdateDir(player *p, vec3 dir)
{
    p->dir.x = dir.x;
    p->dir.y = dir.y;
    p->dir.z = dir.z;
    normalizeVec(&p->dir, p->pos);
}

/**
 * playerUpdatePos - 
 * 
 */
void playerUpdatePos(player *p, vec3 pos)
{
    float velocity = 0.5f;

    p->pos.x = pos.x * velocity;
    p->pos.y = pos.y * velocity;
    p->pos.z = pos.z * velocity;
}

/**
 * renderPlayer -
 * 
 */
void playerRender(sdl_window *w, player *p)
{
    vec3 highPoint = p->dir;
    vec3 lowPoint = p->dir;
    float deltaY;
    
    SDL_SetRenderDrawColor(w->renderer, 255, 0, 0, 1);
    SDL_RenderDrawLine(w->renderer, p->pos.x, p->pos.y, p->dir.x, p->dir.y); // for now we are multiplying for 100 to be able to see better the player vector

    /* render its multiple rays */

    // set y component / 2 as delta
    deltaY = getVecFromAngle(100, p->fov).y / 2;

    // update basis vectors
    highPoint.y += deltaY;
    lowPoint.y -= deltaY;

    //normalizeVec(&highPoint, p->pos);
    //normalizeVec(&lowPoint, p->pos);

    // store array of vec3 with end points for diff rays
    p->noRays = 3;
    p->rays[0] = p->dir;
    p->rays[1] = highPoint;
    p->rays[2] = lowPoint;
}
