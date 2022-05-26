#ifndef MYMATHLIB_HPP
#define MYMATHLIB_HPP


#include "macros.hpp"

class vec2
{
public:
    float x;
    float y;

    // empty constructor
    vec2();

    // full constructor
    vec2(float _x, float _y);

    // overloading operators
    vec2 operator+(vec2 const &obj);
    vec2 operator-(vec2 const &obj);
    vec2 operator*(vec2 const &obj);

};

void normalizeVec(vec2 *vec, vec2 origin);
vec2 getVecFromAngle(float magnitude, float angle);
float distanceBtwPoints(vec2 p1, vec2 p2);

template <typename T>
T clamp(T value, T lower, T upper)
{
    T result;
    result = (value > upper) ? upper : value;
    result = (value < lower) ? lower : value;
    return result;
}

class line
{
public:
    vec2 p1;
    vec2 p2;
};

#endif
