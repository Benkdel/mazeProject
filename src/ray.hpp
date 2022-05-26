#ifndef RAY_HPP
#define RAY_HPP

#include "myMathLib.hpp"

class Ray {
public:

    // Empty constructor
    Ray();

    Ray(vec2 pos, vec2 dir);

    bool cast(line wall);

    vec2 pos;
    vec2 dir;
    vec2 hit;
};

#endif
