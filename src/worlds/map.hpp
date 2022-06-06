#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>
#include <vector>
#include "../sdl_engine/window.hpp"
#include "../macros.hpp"


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
    
    void setPerimeter(SDL_Rect *port);

    void renderGrid(Window *w, SDL_Rect *port);
    void renderInnerWalls(Window *w, SDL_Rect *port);

    SDL_Rect recWalls;

    Cell mapCells[GRID_HEIGHT][GRID_WIDTH / 2];
    std::vector<SDL_Rect> innerWalls;

};

#endif