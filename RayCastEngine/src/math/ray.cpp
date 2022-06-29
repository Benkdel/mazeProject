#include "ray.hpp"

#include <cmath>

Ray::Ray() {}

Ray::~Ray() {}

HitResult Ray::castDDD(vec2 start, vec2 dir, Map *map)
{
    vec2 hitCoords = { -1.0f, -1.0f };
    Axis axisHit = Axis::HORIZONTAL;

    vec2 scalingFactors;
    scalingFactors.x = sqrt(1 + (dir.y / dir.x) * (dir.y / dir.x));
    scalingFactors.y = sqrt(1 + (dir.x / dir.y) * (dir.x / dir.y));

    // keep track of where in the map we are
    int cell_X = start.x / map->getCellSize();
    int cell_Y = start.y / map->getCellSize();

    vec2 rayLength2D; // store length of ray in x and y
    vec2 step;

    // set step to right direction
    step.x = (dir.x < 0) ? -1 : 1;
    step.y = (dir.y < 0) ? -1 : 1;

    // manually calculate first length to closest cell
    if (step.x < 0)
        rayLength2D.x = (start.x - float(cell_X * map->getCellSize())) * scalingFactors.x;
    else
        rayLength2D.x = (float((cell_X + 1) * map->getCellSize()) - start.x) * scalingFactors.x;
    if (step.y < 0)
        rayLength2D.y = (start.y - float(cell_Y * map->getCellSize())) * scalingFactors.y;
    else
        rayLength2D.y = (float((cell_Y + 1) * map->getCellSize()) - start.y) * scalingFactors.y;

    // loop from start to length of the array
    bool tileFound = false;
    float totalDistance = 0.0f;
    while (!tileFound && totalDistance < map->getMaxRayL())
    {
        if (rayLength2D.x < rayLength2D.y)
        {
            cell_X += step.x;
            totalDistance = rayLength2D.x;
            rayLength2D.x += float(scalingFactors.x * map->getCellSize());
            axisHit = Axis::HORIZONTAL;
        }
        else
        {
            cell_Y += step.y;
            totalDistance = rayLength2D.y;
            rayLength2D.y += float(scalingFactors.y * map->getCellSize());
            axisHit = Axis::VERTICAL;
        }

        if (cell_Y >= 0 && cell_X >= 0 && cell_Y <= map->getHeight(), cell_X <= map->getWidth())
        {
            if (map->mMapCells[cell_Y * map->getWidth() + cell_X].value != 0)
            {
                tileFound = true;
                hitCoords.x = start.x + dir.x * totalDistance;
                hitCoords.y = start.y + dir.y * totalDistance;
            }
        }
    }

    HitResult hr = { tileFound, hitCoords, axisHit, totalDistance};

    return hr;
}
