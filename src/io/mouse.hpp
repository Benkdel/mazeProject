#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <SDL.h>
#include "../math/myMathLib.hpp"

class Mouse {
public:
    
    Mouse(double scr_W, double scr_H);

    double scr_W;
    double scr_H;

    double centerX;
    double centerY;

    vec2 position;
    vec2 lastPos;

    void setPosition(int x, int y);
    void getDeltas();

    bool firstMouse;
    bool printData;

};

#endif