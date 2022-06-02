#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "window.hpp"
#include "map.hpp"
#include "myMathLib.hpp"
#include "ray.hpp"

#include <vector>

class Player {
public:

    Player();

    void init(Map *w);

    void updatePos(float velocit, vec2 mousePos, float dt, SDL_Rect *port);

    void render(Window *w, SDL_Rect *port);

    void setUpRays();

    void clearRays();

    std::vector<Ray> rays;

    vec2 pos;
    vec2 velocity;
    vec2 lastVelocity; // debuging
    vec2 dir;

    SDL_Rect box_collider;

    // if we want to draw player as circle
    float radius;
    // as triangle:
    Triangle triangle;
    Triangle translTriangle; // for debugging

    float angle;
};

#endif