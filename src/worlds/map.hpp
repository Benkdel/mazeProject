#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>
#include <vector>

#include "../macros.hpp"
#include "../sdl_engine/window.hpp"
#include "../math/myMathLib.hpp"

class Cell {
public:
    SDL_Rect rect;
    char value;
};

class Map {
public:
    
    // Empty constructor
    Map();

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    
    void setWalls();
    void renderMiniMap(Window *w, SDL_Rect *VPminimap, SDL_Rect *VPworld);

    SDL_Rect recWalls;
    Cell mapCells[GRID_HEIGHT][GRID_WIDTH];

    Cell getCell(vec2 vec);
};

#endif