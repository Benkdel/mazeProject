#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>
#include "window.hpp"

class OuterWalls {
public:
    
    // Empty constructor
    OuterWalls();

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    
    void setPerimeter(SDL_Rect *port, double size);

    void render(Window *w, SDL_Rect *port);

    SDL_Rect recWalls;

};

#endif