#ifndef RAY_HPP
#define RAY_HPP

#include <SDL.h>
#include "myMathLib.hpp"
#include "map.hpp"

struct HitResult {
    bool hit;
    vec2 intersection;
};

class Ray {
public:

    // Empty constructor
    Ray();

    Ray(vec2 pos, vec2 dir);

    bool cast(SDL_Rect wall);
    HitResult castDDD(vec2 pos, vec2 lookAt, Map *map);

    vec2 pos;
    vec2 dir;
    vec2 hit;
};

#endif
