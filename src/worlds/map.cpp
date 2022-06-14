#include "map.hpp"

#include <string>
#include <vector>
#include <algorithm>

Map::Map() {}


void Map::renderMiniMap(Window *w, SDL_Rect *VPminimap, SDL_Rect *VPworld)
{
    // render grid
    int x, y;

    SDL_RenderSetViewport(w->renderer, VPminimap);

    // render vertical lines
    SDL_SetRenderDrawColor(w->renderer, 220, 236, 225, 100);

    float xConvRatio = ((float)VPminimap->w / (float)VPworld->w);
    float yConvratio = ((float)VPminimap->h / (float)VPworld->h);
    
    float cellWidth = (float)CELL_SIZE * xConvRatio;
    float cellHeight = (float)CELL_SIZE * yConvratio;

    for (x = cellWidth; x < 1 + cellWidth * GRID_WIDTH; x += cellWidth)
    {
        SDL_RenderDrawLineF(w->renderer, x, 0, x, VPminimap->h);
    }

    // render horizontal lines
    SDL_SetRenderDrawColor(w->renderer, 220, 236, 225, 100);
    for (y = cellHeight; y < 1 + cellHeight * GRID_HEIGHT; y += cellHeight)
    {
        SDL_RenderDrawLineF(w->renderer, 0, y, VPminimap->w, y);
    }

    // render walls
    SDL_SetRenderDrawColor(w->renderer, 0, 204, 204, 255);
    SDL_Rect worldCell;
    for (int r = 0; r < GRID_HEIGHT; r++)
    {
        for (int c = 0; c < GRID_WIDTH; c++)
        {
            if (mapCells[r][c].value == '#')
            {
                worldCell = mapCells[r][c].rect;
                worldCell.x *= xConvRatio;
                worldCell.y *= yConvratio;
                worldCell.w = cellWidth;
                worldCell.h = cellHeight;
                SDL_RenderFillRect(w->renderer, &worldCell);
            }
        }
    }
}

void Map::setWalls()
{
    // set perimeter
    this->x = 0;
    this->y = 0;
    this->w = GRID_WIDTH * CELL_SIZE;
    this->h = GRID_HEIGHT * CELL_SIZE;

    // set map
    float columns = GRID_WIDTH;
    float rows = GRID_HEIGHT;

    std::string map_grid[GRID_HEIGHT] = {
        {" ######################################"},
        {" #        #              #            #"},
        {" #        #                     #     #"},
        {" #             #      #     #   #     #"},
        {" #   #   ##############     #   #     #"},
        {" #   #     #          #     #   #     #"},
        {" #   #     #    #     #     #   #     #"},
        {" #   #          #     #     #   #     #"},
        {" #   #  #########     #######   #######"},
        {" #   #  #       #                     #"},
        {" #   #  #   #   #      ################"},
        {" #   #  #   # # #                     #"},
        {" #                   #                #"},
        {" #                  #                 #"},
        {" #################################    #"},
        {" #     #              #               #"},
        {" #     #         #                 ## #"},
        {" #  #########    #    #############   #"},
        {" #          #    #    #        #      #"},
        {" #    #     #    #    #        #    ###"},
        {" #    #     #         #        #      #"},
        {" #    ##### ####      #        ####   #"},
        {" #                    #        #      #"},
        {" #        #     #######        #    ###"},
        {" #    #####           #        #      #"},
        {" #           ####     #        ####   #"},
        {" #                    ##########      #"},
        {" #        #####            #          #"},
        {" #                    #               #"},
        {" ######################################"}};
    
    int x1, y1;
    int r = 0, lR = 0, c = 0, lC = 0;

    for (r; r < rows; r++)
    {
        for (c; c < columns; c++)
        {
            x1 = lC * CELL_SIZE;
            y1 = lR * CELL_SIZE;
            
            this->mapCells[r][c].rect = {x1, y1, CELL_SIZE, CELL_SIZE};
            
            if (map_grid[r][c] == '#')
                this->mapCells[r][c].value = '#';
            else
                this->mapCells[r][c].value = ' ';

            lC = c;
        }
        lC = 0;
        c = 0;
        lR = r;
    }
}
