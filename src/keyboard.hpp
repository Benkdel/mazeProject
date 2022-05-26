#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "myMathLib.hpp"
#include <stdint.h>
#include <cstdint>

class Keyboard {
public:

    Keyboard();

    uint8_t *currentKeyState;
    vec2 pos;
};


#endif