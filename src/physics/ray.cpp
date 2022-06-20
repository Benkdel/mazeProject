#include "ray.hpp"

#include <cmath>

Ray::Ray() {}

void Ray::castDDD(vec2 pos, vec2 lookAt, Map *map)
{
    vec2 rayStart = pos;
    vec2 rayDir = lookAt;

    // compute scaling factors of c when moving 1 unit in y, and same for 1 unit in x
    vec2 scalingFactors;
    scalingFactors.x = sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x));
    scalingFactors.y = sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y));

    // keep track of where in the map we are
    int cell_X = pos.x / map->getCellSize();
    int cell_Y = pos.y / map->getCellSize();

    vec2 rayLength2D; // store length of ray in x and y
    vec2 step;

    // set step to right direction
    step.x = (rayDir.x < 0) ? -1 : 1;
    step.y = (rayDir.y < 0) ? -1 : 1;

    // manually calculate first length to closest cell
    if (step.x < 0)
        rayLength2D.x = (rayStart.x - float(cell_X * map->getCellSize())) * scalingFactors.x;
    else
        rayLength2D.x = (float((cell_X + 1) * map->getCellSize()) - rayStart.x) * scalingFactors.x;
    if (step.y < 0)
        rayLength2D.y = (rayStart.y - float(cell_Y * map->getCellSize())) * scalingFactors.y;
    else
        rayLength2D.y = (float((cell_Y + 1) * map->getCellSize()) - rayStart.y) * scalingFactors.y;

    // loop from start to length of the array
    bool tileFound = false;
    float maxDistance = map->getMaxRayL();
    float totalDistance = 0.0f;
    while (!tileFound && totalDistance < maxDistance)
    {
        // walk along the shortest direction
        if (rayLength2D.x < rayLength2D.y)
        {
            cell_X += step.x;
            totalDistance = rayLength2D.x;
            rayLength2D.x += float(scalingFactors.x * map->getCellSize());
            this->results.HitDir = 0; // horizontal hit
        }
        else
        {
            cell_Y += step.y;
            totalDistance = rayLength2D.y;
            rayLength2D.y += float(scalingFactors.y * map->getCellSize());
            this->results.HitDir = 1; // vertical hit
        }

        if (cell_Y >= 0 && cell_X >= 0 && cell_Y <= map->getHeight(), cell_X <= map->getWidth())
        {
            if (map->mMapCells[cell_Y * map->getWidth() + cell_X].value == '#')
            {
                tileFound = true;
            }
        }
    }

    /* calculate intersection hit */
    this->results.intersection.x = rayStart.x + rayDir.x * totalDistance;
    this->results.intersection.y = rayStart.y + rayDir.y * totalDistance;

    if (tileFound)
    {
        this->distance = totalDistance;
        this->results.hit = true;
    }
}
