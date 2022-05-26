#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>
#include "window.hpp"
#include <vector>
#include "macros.hpp"

class Map {
public:
    
    // Empty constructor
    Map();

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    
    void setPerimeter(SDL_Rect *port);

    void renderGrid(Window *w, SDL_Rect *port);
    void renderInnerWalls(Window *w, SDL_Rect *port);

    SDL_Rect recWalls;

    SDL_Rect mapCells[GRID_HEIGHT][GRID_WIDTH];
    std::vector<SDL_Rect> innerWalls;

};

#endif