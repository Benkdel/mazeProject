#ifndef RAY_HPP
#define RAY_HPP

#include <SDL.h>
#include "../math/myMathLib.hpp"
#include "../worlds/map.hpp"

struct HitResult {
    bool hit;
    vec2 intersection;
    int HitDir;
};

class Ray {
public:

    // Empty constructor
    Ray();

    void castDDD(vec2 pos, vec2 rayDir, Map *map);

    HitResult results;
    vec2 rayDir;
    float distance;
    float wallHeight;
    float angle;
};

#endif
