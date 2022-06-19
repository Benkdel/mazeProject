#ifndef MYMATHLIB_HPP
#define MYMATHLIB_HPP

#include "../macros.hpp"

#include <iostream>

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
    bool operator==(vec2 const &obj);
    bool operator!=(vec2 const &obj);
    float operator*(vec2 const &obj);
};

vec2 normalizeVec(vec2 vec, vec2 origin);
vec2 getVecFromAngle(float magnitude, float angle);
float distanceBtwPoints(vec2 p1, vec2 p2);
float getAngleFromVectors(vec2 v1, vec2 v2);
float FixAng(float a);

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
    line();

    line(vec2 p1, vec2 p2);

    vec2 p1;
    vec2 p2;
};

class Triangle
{
public:
    Triangle();

    Triangle(vec2 p1, vec2 p2, vec2 p3);

    vec2 p1;
    vec2 p2;
    vec2 p3;
};

// matrix transforms
vec2 rotate2Dvec(vec2 v, float angle);
vec2 scale2Dvec(vec2 v, float size);

// utilities
float deg2rad(float angle);
float rad2deg(float radians);

#endif
