#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "../sdl_engine/window.hpp"
#include "../worlds/map.hpp"
#include "../math/mathlib.hpp"
#include "../math/ray.hpp"
#include "../io/keyboard.hpp"
#include "../io/mouse.hpp"

#include <vector>


class Player {
public:

    Player();
    void init(Map *w);
    void updatePos(Keyboard *kb, float dt, Map *map);
    void updateCurrentAngle(Mouse *m, float dt);
    void translate(int size, int numRays);
   
    vec2 pos;
    vec2 lookAt;
        
    // as triangle:
    Triangle triangle;
    Triangle transfTriangle;

    float angle;
};

#endif