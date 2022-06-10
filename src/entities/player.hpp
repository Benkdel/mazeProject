#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "../sdl_engine/window.hpp"
#include "../worlds/map.hpp"
#include "../math/myMathLib.hpp"
#include "../physics/ray.hpp"
#include "../io/keyboard.hpp"

#include <vector>

class Player {
public:

    Player();
    void init(Map *w);
    void updatePos(Keyboard *kb, float dt);
    void updateCurrentAngle(Keyboard *kb, float dt);
    void render(Window *w, SDL_Rect *port);
    void rayCastDDD(Window *window, SDL_Rect *port, Map *map);
    void clearRays();

    Ray rays[MAX_RAYS];

    vec2 pos;
    vec2 lookAt;
    
    SDL_Rect box_collider;

    // if we want to draw player as circle
    float radius;
    
    // as triangle:
    Triangle triangle;

    float angle;
};

#endif