#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <SDL.h>
#include "../math/myMathLib.hpp"

class Mouse {
public:
    
    Mouse();

    vec2 position;

    void setPosition(int x, int y);

};

#endif