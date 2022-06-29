#ifndef RAY_HPP
#define RAY_HPP

#include <SDL.h>
#include "mathlib.hpp"
#include "../worlds/map.hpp"

enum class Axis {
    HORIZONTAL = 0, VERTICAL
};

struct HitResult {
    bool hit;
    vec2 hitCoords;
    Axis axisHit;
    float distance;
};

class Ray {
public:

    // Empty constructor
    Ray();

    // destructor
    ~Ray();

    static HitResult castDDD(vec2 pos, vec2 dir, Map *map);
};

#endif
