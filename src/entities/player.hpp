#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "../sdl_engine/window.hpp"
#include "../worlds/map.hpp"
#include "../math/myMathLib.hpp"
#include "../physics/ray.hpp"

#include <vector>

class Player {
public:

    Player();
    void init(Map *w);
    void updatePos(float acceleration, vec2 mousePos, float dt, SDL_Rect *port);
    void updateCurrentAngle(float rotAngle);
    void render(Window *w, SDL_Rect *port);
    void setUpRays();
    void rayCastDDD(Window *window, SDL_Rect *port, Map *map);
    void clearRays();

    Ray rays[MAX_RAYS];

    vec2 pos;
    vec2 velocity;
    vec2 lookAt;

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