#include "headers.h"

/**
 * renderScene - 
 * 
 * 
 */
void renderScene(sdl_window *w, SDL_Rect *port, const line *l, size_t sceneLength)
{
    SDL_Rect wallSlice;
    float sliceWidth = port->w / sceneLength;
    size_t i;
    float raysLen = 0.0f;
    vec3 p1;
    vec3 p2;

    int R = 0;
    int G = 255;
    int B = 51;

    int maxDistance = 200; // change this later

    SDL_RenderSetViewport(w->renderer, port);
    
    for (i = 0; i < sceneLength; i++)
    {
        p1.x = l[i].x1;
        p1.y = l[i].y1;
        p1.z = 0.0f;

        p2.x = l[i].x2;
        p2.y = l[i].y2;
        p2.z = 0.0f;

        raysLen = distanceBtwPoints(p1, p2);
        
        wallSlice.x = sliceWidth * i; //start in next pos evert loop
        wallSlice.y = port->y;
        wallSlice.w = raysLen;
        wallSlice.h = port->h;

        R = 0 + (raysLen * 255 / maxDistance);
        B = 51 + (raysLen * 204 / maxDistance); // 204 is dif btw 255 (no color) 51 (Green)

        clampInts(&R, 0, 255);
        clampInts(&B, 51, 255);

        SDL_SetRenderDrawColor(w->renderer, R, G, B, 1);
        SDL_RenderFillRect(w->renderer, &wallSlice);
    }
}
