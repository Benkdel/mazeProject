#include "collision.hpp"
#include "macros.hpp"
#include <cmath>
#include <iostream>

// ========================================================================
// algorith taken from Kofybrek and adjusted to work with this project
// https://github.com/Kofybrek/Raycasting/blob/Main/Source/MapCollision.cpp
// ========================================================================

bool map_collision(vec2 pos, Map *map, SDL_Rect *port, Window *w)
{
    float cell_x = pos.x / CELL_SIZE;
    float cell_y = pos.y / CELL_SIZE;

    SDL_Rect hoverGrid;

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

        if (0 <= x && 0 <= y && port->h > y && port->w > x)
        {
            if (map->mapCells[y][x].value == '#')
            {
                hoverGrid = {((int)pos.x / CELL_SIZE) * CELL_SIZE, ((int)pos.y / CELL_SIZE) * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderSetViewport(w->renderer, port);
                SDL_SetRenderDrawColor(w->renderer, 255, 255, 0, 1);
                SDL_RenderFillRect(w->renderer, &hoverGrid);
                std::cout << "Collision detected \n Player pos: x: " << pos.x << " y: " << pos.y << "\n" << "Wall at: x: " << x << " y: " << y << "\n";
                return 1;
            }
        }
    }
    return 0;
}

bool map_collision_2(vec2 pos, Map *map, SDL_Rect *port, Window *w)
{
    
    return 0;
}