#include "ray.hpp"

#include <cmath>

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

HitResult Ray::castDDD(vec2 pos, vec2 lookAt, Map *map)
{
    vec2 rayStart = pos;
    vec2 rayDir = normalizeVec(lookAt - pos, vec2(0.0f, 0.0f));

    if (pos.x > 500.0f)
        std::cout << "Debug RayCast ... \n\n";

    // compute scaling factors of c when moving 1 unit in y, and same for 1 unit in x
    vec2 scalingFactors;
    scalingFactors.x = sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x));
    scalingFactors.y = sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y));

    // keep track of where in the map we are
    int cell_X = pos.x / CELL_SIZE;
    int cell_Y = pos.y / CELL_SIZE;

    vec2 rayLength2D; // store length of ray in x and y
    vec2 step;

    // set step to right direction
    step.x = (rayDir.x < 0) ? -1 : 1;
    step.y = (rayDir.y < 0) ? -1 : 1;

    // manually calculate first length to closest cell
    if (step.x < 0)
        rayLength2D.x = (rayStart.x - cell_X * CELL_SIZE) * scalingFactors.x;
    else
        rayLength2D.x = ((cell_X + 1) * CELL_SIZE - rayStart.x) * scalingFactors.x;
    if (step.y < 0)
        rayLength2D.y = (rayStart.y - cell_Y * CELL_SIZE) * scalingFactors.y;
    else
        rayLength2D.y = ((cell_Y + 1) * CELL_SIZE - rayStart.y) * scalingFactors.y;

    // loop from start to length of the array
    bool tileFound = false;
    float maxDistance = 200.0f;
    float totalDistance = 0.0f;
    while (!tileFound && totalDistance < maxDistance)
    {
        // walk along the shortest direction
        if (rayLength2D.x < rayLength2D.y)
        {
            cell_X += step.x;
            totalDistance = rayLength2D.x;
            rayLength2D.x += CELL_SIZE;
        }
        else
        {
            cell_Y += step.y;
            totalDistance = rayLength2D.y;
            rayLength2D.y += CELL_SIZE;
        }

        if (cell_Y >= 0 && cell_X >= 0 && cell_Y <= map->h, cell_X <= map->w)
        {
            if (map->mapCells[cell_Y, cell_X]->value == '#')
            {
                tileFound = true;
            }
        }
    }

    HitResult result;
    result.hit = false;

    result.intersection.x = rayStart.x + rayDir.x * totalDistance;
    result.intersection.y = rayStart.y + rayDir.y * totalDistance;

    if (tileFound)
    {
        result.hit = true;
    }
    return result;
}
