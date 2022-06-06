#include "map.hpp"

#include <string>
#include <vector>
#include <algorithm>

Map::Map() {}

void Map::setPerimeter(SDL_Rect *port)
{
    // store map values(this->window->getWidth() - 1) / 2;
    this->x = 0;
    this->y = 0;
    this->w = port->w - this->x;
    this->h = port->h - this->y;

    // set SDL rect points
    this->recWalls.x = this->x;
    this->recWalls.y = this->y;
    this->recWalls.w = this->w;
    this->recWalls.h = this->h;
}

void Map::renderGrid(Window *w, SDL_Rect *port)
{
    int x, y;

    SDL_RenderSetViewport(w->renderer, port);

    // render vertical lines
    SDL_SetRenderDrawColor(w->renderer, 193, 193, 193, 1);
    for (x = CELL_SIZE; x < 1 + CELL_SIZE * GRID_WIDTH; x += CELL_SIZE)
    {
        if (x == CELL_SIZE * (GRID_WIDTH - 1))
        {
            SDL_SetRenderDrawColor(w->renderer, 111, 149, 179, 1);
        }
        SDL_RenderDrawLine(w->renderer, x, 0, x, this->h + CELL_SIZE);
    }

    // render horizontal lines
    SDL_SetRenderDrawColor(w->renderer, 193, 193, 193, 1);
    for (y = CELL_SIZE; y < 1 + CELL_SIZE * GRID_HEIGHT; y += CELL_SIZE)
    {
        if (y == CELL_SIZE * (GRID_HEIGHT - 1)) 
        {
            SDL_SetRenderDrawColor(w->renderer, 111, 149, 179, 1);
        }
        SDL_RenderDrawLine(w->renderer, 0, y, this->w + CELL_SIZE, y);
    }
}

void Map::renderInnerWalls(Window *w, SDL_Rect *port)
{
    // this map is for the view port, adjust columns and rows
    // to match it
    float columns = port->w / CELL_SIZE;
    float rows = GRID_HEIGHT;

    std::string map_grid[GRID_HEIGHT] = {
        {"####################"},
        {"#                  #"},
        {"#                  #"},
        {"#    ###############"},
        {"#    #             #"},
        {"#    #             #"},
        {"#    #             #"},
        {"#    #             #"},
        {"#    #  ########   #"},
        {"#    #  #      #   #"},
        {"#    #  #   #  #   #"},
        {"#    #  #   #  #   #"},
        {"#    ####   #  #   #"},
        {"#           #      #"},
        {"#           ########"},
        {"#                  #"},
        {"#                  #"},
        {"#                  #"},
        {"#                  #"},        
        {"#                  #"},        
        {"#                  #"},        
        {"#                  #"},        
        {"#                  #"},        
        {"#         #        #"},        
        {"#     #####        #"},        
        {"#                  #"},        
        {"#                  #"},        
        {"#         #####    #"},        
        {"#                  #"},        
        {"####################"}};

    int x1, y1;
    int r = 0, lR = 0, c = 0, lC = 0;

    // size of each cell map
    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 0, 255, 51, 1);

    this->innerWalls.clear();
    for (r; r < rows; r++)
    {
        for (c; c < columns; c++)
        {
            x1 = lC * CELL_SIZE;
            y1 = lR * CELL_SIZE;
            this->mapCells[r][c].rect = {x1, y1, CELL_SIZE, CELL_SIZE};
            this->mapCells[r][c].value = ' ';

            if (map_grid[r][c] == '#')
            {
                this->innerWalls.push_back(this->mapCells[r][c].rect);
                this->mapCells[r][c].value = '#';
                SDL_RenderFillRect(w->renderer, &this->mapCells[r][c].rect);
            }
            lC = c;
        }
        lC = 0;
        c = 1;
        lR = r;
    }
}
