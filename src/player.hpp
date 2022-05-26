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

    void updateDir(vec2 dir);
    void updatePos(vec2 pos);

    void render(Window *w, SDL_Rect *port);

    void setUpRays();

    void clearRays();

    std::vector<Ray> rays;

    vec2 pos;
    vec2 dir;
};

#endif