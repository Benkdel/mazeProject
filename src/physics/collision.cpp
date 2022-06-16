#include "collision.hpp"
#include "../macros.hpp"

#include <cmath>
#include <iostream>

vec2 map_collision(vec2 pos, vec2 newPos, vec2 velocity, Map *map)
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
