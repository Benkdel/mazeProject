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

vec2 map_collision_2(vec2 pos, std::vector<vec2> box, Map *map)
{
    vec2 dist = {-1.0f, -1.0f};

    int cell_X = pos.x / CELL_SIZE;
    int cell_Y = pos.y / CELL_SIZE;

    bool left = map->mapCells[cell_Y][cell_X - 1].value == '#';
    bool top = map->mapCells[cell_Y - 1][cell_X].value == '#';
    bool right = map->mapCells[cell_Y][cell_X + 1].value == '#';
    bool bottom = map->mapCells[cell_Y + 1][cell_X].value == '#';

    // std::cout << "Left: " << ((left == 1) ? "WALL" : "SPACE") << "\n";
    // std::cout << "Top: " << ((top == 1) ? "WALL" : "SPACE") << "\n";
    // std::cout << "Right: " << ((right == 1) ? "WALL" : "SPACE") << "\n";
    // std::cout << "Bottom: " << ((bottom == 1) ? "WALL" : "SPACE") << "\n";

    if (left)
    {
        float x = map->mapCells[cell_Y][cell_X - 1].rect.x + CELL_SIZE;
        float closest = 100.0f;
        for (int i = 0; i < 4; i++)
        {
            float current = box[i].x - x;
            if (current < closest)
                closest = current;
        }
        dist.x = closest;
    }

    if (top)
    {
        float y = map->mapCells[cell_Y - 1][cell_X].rect.y + CELL_SIZE;
        float closest = 100.0f;
        for (int i = 0; i < 4; i++)
        {
            float current = box[i].y - y;
            if (current < closest)
                closest = current;
        }
        dist.y = closest;
    }

    if (right)
    {
        float x = map->mapCells[cell_Y][cell_X + 1].rect.x + CELL_SIZE;
        float closest = 100.0f;
        for (int i = 0; i < 4; i++)
        {
            float current = x - box[i].x;
            if (current < closest)
                closest = current;
        }
        dist.x = closest;
    }

    if (bottom)
    {
        float y = map->mapCells[cell_Y + 1][cell_X].rect.y - CELL_SIZE;
        float closest = 100.0f;
        for (int i = 0; i < 4; i++)
        {
            float current = y - box[i].y;
            if (current < closest)
                closest = current;
        }
    }

    return dist;
}
