#include "ray.hpp"

Ray::Ray() {}

Ray::Ray(vec2 pos, vec2 dir)
    : pos(pos), dir(dir) {}

bool Ray::cast(SDL_Rect wall)
{
    float t = -1; // must be betw 0 and 1
    float u = -1; // must be > 0

    // use wiky notation
    const float x1 = wall.x;
    const float y1 = wall.y;
    const float x2 = wall.x + wall.w;
    const float y2 = wall.y + wall.h;
    const float x3 = this->pos.x;
    const float y3 = this->pos.y;
    const float x4 = this->dir.x;
    const float y4 = this->dir.y;

    float den;

    den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den != 0)
    {
        t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
        u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

        if (t > 0 && t < 1 && u > 0)
        {
            this->hit.x = x1 + t * (x2 - x1);
            this->hit.y = y1 + t * (y2 - y1);
            return (true);
        }
        else
        {
            return (false);
        }
    }
    return (false);
}

bool Ray::castDDD(Map *map)
{
    
}