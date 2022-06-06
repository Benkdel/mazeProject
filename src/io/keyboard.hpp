#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "../math/myMathLib.hpp"
#include <stdint.h>
#include <cstdint>

class Keyboard {
public:

    Keyboard();

    uint8_t *currentKeyState;
    vec2 pos;

    float velocity;
    float acceleration;

    float angle;

    bool called;

    bool printData;
};


#endif