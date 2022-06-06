#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <SDL.h>

class Mouse {
public:
    
    Mouse();

    SDL_Point position;

    void setPosition(int x, int y);

};

#endif