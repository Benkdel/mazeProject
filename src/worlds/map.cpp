#include "map.hpp"

#include <string>
#include <vector>
#include <algorithm>


Map::Map() {}


void Map::renderMiniMap(Window *w, SDL_Rect *VPminimap, SDL_Rect *VPworld)
{
    SDL_RenderSetViewport(w->renderer, VPminimap);

    float xConvRatio = ((float)VPminimap->w / (float)VPworld->w);
    float yConvratio = ((float)VPminimap->h / (float)VPworld->h);
    
    float cellWidth = (float)CELL_SIZE * xConvRatio;
    float cellHeight = (float)CELL_SIZE * yConvratio;

    // render walls
    SDL_Rect worldCell;
    for (int r = 0; r < GRID_HEIGHT; r++)
    {
        for (int c = 0; c < GRID_WIDTH; c++)
        {
            worldCell = mapCells[r][c].rect;
            worldCell.x *= xConvRatio;
            worldCell.y *= yConvratio;
            worldCell.w = cellWidth;
            worldCell.h = cellHeight;
                
            if (mapCells[r][c].value == '#')
            {
                SDL_SetRenderDrawColor(w->renderer, 0, 204, 204, 255);
                SDL_RenderFillRect(w->renderer, &worldCell);
            }
            else
            {
                SDL_SetRenderDrawColor(w->renderer, 0, 204, 204, 85);            
                SDL_RenderDrawRect(w->renderer, &worldCell);
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
        {"#######################################"},
        {"#    #    #                           #"},
        {"#    #    #    #                #     #"},
        {"#              #      #     #   #     #"},
        {"#    #   ##############     #   #     #"},
        {"#    #     #          #     #   #     #"},
        {"#    #     #    #     #     #   #     #"},
        {"#    #          #     #     #   #     #"},
        {"#    #  #########     #######   #######"},
        {"#    #  #       #                     #"},
        {"#    #  #   #   #      ################"},
        {"#    #  #   # # #        #            #"},
        {"#                   #    #    #       #"},
        {"#       #           #         #       #"},
        {"# ################################    #"},
        {"#      #              #         #     #"},
        {"#      #         #           #     ####"},
        {"#   #########    #    #############   #"},
        {"#           #    #    #        #      #"},
        {"#     #     #    #    #        #    ###"},
        {"#     #     #         #        #      #"},
        {"#     ##### ####      #        ####   #"},
        {"#                     #        #      #"},
        {"#         #     #######        #    ###"},
        {"#     #####           #        #      #"},
        {"#            ####     #        ####   #"},
        {"#     #      #        ##########      #"},
        {"#     #   #####    #           #      #"},
        {"#     #          ###      #           #"},
        {"#######################################"}};

    int x1, y1;
    int r = 0, lR = 0, c = 0, lC = 0;

    for (r; r < rows; r++)
    {
        for (c; c < columns; c++)
        {
            x1 = c * CELL_SIZE;
            y1 = r * CELL_SIZE;
            
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

Cell Map::getCell(vec2 vec)
{
    int cell_X = vec.x / CELL_SIZE;
    int cell_Y = vec.y / CELL_SIZE;

    return this->mapCells[cell_Y][cell_X];
}