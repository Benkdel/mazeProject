#include "collision.hpp"
#include "../macros.hpp"

#include <cmath>
#include <iostream>

// ========================================================================
// algorith taken from Kofybrek and adjusted to work with this project
// https://github.com/Kofybrek/Raycasting/blob/Main/Source/MapCollision.cpp
// ========================================================================

vec2 map_collision(vec2 pos, Map *map)
{
    float cell_x = pos.x / CELL_SIZE;
    float cell_y = pos.y / CELL_SIZE;

    vec2 coll = vec2(-1.0f, -1.0f);

    for (unsigned char a = 0; a < 4; a++)
    {
        short x = 0;
        short y = 0;

        switch (a)
        {
        case 0:
        {
            x = static_cast<short>(floor(cell_x));
            y = static_cast<short>(floor(cell_y));

            break;
        }
        case 1:
        {
            x = static_cast<short>(ceil(cell_x));
            y = static_cast<short>(floor(cell_y));

            break;
        }
        case 2:
        {
            x = static_cast<short>(floor(cell_x));
            y = static_cast<short>(ceil(cell_y));

            break;
        }
        case 3:
        {
            x = static_cast<short>(ceil(cell_x));
            y = static_cast<short>(ceil(cell_y));
        }
        }

        if (map->mapCells[y][x].value == '#')
        {
            coll = {pos.x = map->mapCells[y][x].rect.x, pos.y = map->mapCells[y][x].rect.y};
            // std::cout << "Collision detected \n Player pos: x: " << p->pos.x << " y: " << p->pos.y << "\n"
            //     << "Wall at: x: " << x << " y: " << y << "\n";
        }
    }
    return coll;
}


vec2 map_collision_2(vec2 pos, vec2 newPos, vec2 velocity, Map *map)
{
    vec2 dist = {-1.0f, -1.0f};

    /* 
    ===================================
        Check for Horizontal Collision
    ===================================
    */

    // going to the left
    if (velocity.x <= 0)
    {
        if (map->getCell(vec2(newPos.x, pos.y)).value == '#' || map->getCell(vec2(newPos.x, pos.y + 0.99f)).value == '#')
            dist.x = (int)pos.x + 1;
    }
    // going to the right
    else
    {
        if (map->getCell(vec2(newPos.x + 1.0f, pos.y)).value == '#' || map->getCell(vec2(newPos.x + 1.0f, pos.y + 0.99f)).value == '#')
            dist.x = (int)pos.x;
    }
        
    /* 
    ===================================
        Check for Vertical Collision
    ===================================
    */

    // going Up
    if (velocity.y <= 0)
    {
        if (map->getCell(vec2(newPos.x, newPos.y)).value == '#' || map->getCell(vec2(newPos.x + 1.0f, newPos.y)).value == '#')
            dist.y = (int)pos.y + 1;
    }
    // going down
    else
    {
        if (map->getCell(vec2(newPos.x, newPos.y + 1.0f)).value == '#' || map->getCell(vec2(newPos.x + 1.0f, newPos.y + 1.0f)).value == '#')
            dist.y = (int)pos.y;
    }

    return dist;
}
