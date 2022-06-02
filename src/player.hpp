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
    void updatePos(float acceleration, vec2 mousePos, float dt, SDL_Rect *port);
    void render(Window *w, SDL_Rect *port);
    void setUpRays();
    void clearRays();

    std::vector<Ray> rays;

    vec2 pos;
    vec2 velocity;

    float horizontalMaxSpeed;
    float verticalMaxSpeed;
    float slowdownRate;

    float velocity_mag;
    float prevVelocity_mag;
    
    SDL_Rect box_collider;

    // if we want to draw player as circle
    float radius;
    
    // as triangle:
    Triangle triangle;

    float angle;
    
    // for debugging
    vec2 lastVelocity;
    Triangle translTriangle;
};

#endif