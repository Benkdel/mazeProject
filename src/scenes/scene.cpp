#include "scene.hpp"

Scene::Scene() {}

void Scene::render(Window *w, SDL_Rect *port, std::vector<line> scene)
{
    SDL_Rect wallSlice;
    float sliceWidth = port->w / scene.size();
    size_t i;
    float raysLen = 0.0f;
    vec2 p1;
    vec2 p2;

    int R = 0;
    int G = 255;
    int B = 51;

    int maxDistance = 200; // change this later

    SDL_RenderSetViewport(w->renderer, port);

    for (i = 0; i < scene.size(); i++)
    {
        p1.x = scene[i].p1.x;
        p1.y = scene[i].p1.y;

        p2.x = scene[i].p2.x;
        p2.y = scene[i].p2.y;

        raysLen = distanceBtwPoints(p1, p2);

        wallSlice.x = sliceWidth * i; // start in next pos evert loop
        wallSlice.y = port->y;
        wallSlice.w = raysLen;
        wallSlice.h = port->h;

        R = 0 + (raysLen * 255 / maxDistance);
        B = 51 + (raysLen * 204 / maxDistance); // 204 is dif btw 255 (no color) 51 (Green)

        //R = clamp(R, 0, 255);
        //B = clamp(B, 51, 255);

        SDL_SetRenderDrawColor(w->renderer, R, G, B, 1);
        SDL_RenderFillRect(w->renderer, &wallSlice);
    }
}
