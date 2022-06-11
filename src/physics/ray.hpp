#ifndef RAY_HPP
#define RAY_HPP

#include <SDL.h>
#include "../math/myMathLib.hpp"
#include "../worlds/map.hpp"

struct HitResult {
    bool hit;
    vec2 intersection;
};

class Ray {
public:

    // Empty constructor
    Ray();

    HitResult castDDD(vec2 pos, vec2 lookAt, Map *map);

    vec2 rayDir;
    float distance;
};

#endif
