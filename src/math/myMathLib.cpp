#include "myMathLib.hpp"

#include <cmath>

vec2::vec2() {}

vec2::vec2(float _x, float _y)
    : x(_x), y(_y){};

vec2 vec2::operator+(vec2 const &obj)
{
    vec2 res;
    res.x = x + obj.x;
    res.y = y + obj.y;
    return res;
}

vec2 vec2::operator-(vec2 const &obj)
{
    vec2 res;
    res.x = x - obj.x;
    res.y = y - obj.y;
    return res;
}

bool vec2::operator==(vec2 const &obj)
{
    vec2 res;
    if (res.x == obj.x && res.y == obj.y)
        return true;
    else
        return false;
}

bool vec2::operator!=(vec2 const &obj)
{
    vec2 res;
    if (res.x != obj.x && res.y != obj.y)
        return true;
    else
        return false;
}

float vec2::operator*(vec2 const &obj)
{
    float res;
    res = x * obj.x;
    res += y * obj.y;
    return res;
}

vec2 normalizeVec(vec2 vec, vec2 origin)
{
    vec2 normVec;
    float x = vec.x - origin.x;
    float y = vec.y - origin.y;
    float magnitude;

    magnitude = sqrt(x * x + y * y);

    if (magnitude > 0)
    {
        // normalize vector:
        normVec.x = vec.x / magnitude;
        normVec.y = vec.y / magnitude;
    }
    return normVec;
}

float getAngleFromVectors(vec2 v1, vec2 v2)
{
    float radians;
    float adjacent = v2.x - v1.x;
    float opposite = v2.y - v1.y;
    radians = atan(opposite / adjacent);

    return rad2deg(radians);
}

vec2 getVecFromAngle(float magnitude, float angle)
{
    vec2 result;
    float radians = deg2rad(angle);

    result.x = cosf(radians) * magnitude;
    result.y = -sinf(radians) * magnitude;

    return result;
}

float distanceBtwPoints(vec2 p1, vec2 p2)
{
    float distX;
    float distY;

    distX = (p1.x - p2.x) * (p1.x - p2.x);
    distY = (p1.y - p1.y) * (p1.y - p2.y);

    return (sqrt(distX + distY));
}

vec2 rotate2Dvec(vec2 v, float angle)
{
    vec2 newVec;
    float radians = deg2rad(angle);

    newVec.x = v.x * cosf(radians) - v.y * sinf(radians);
    newVec.y = v.x * sinf(radians) + v.y * cosf(radians);

    return newVec;
}

vec2 scale2Dvec(vec2 v, float size)
{
    vec2 newVec;

    newVec.x = v.x * size;
    newVec.y = v.y * size;

    return newVec;
}

float clampAngle(float a)
{
    if (a > 359)
    {
        a -= 360;
    }
    if (a < 0)
    {
        a += 360;
    }
    return a;
}

line::line() {}

line::line(vec2 p1, vec2 p2)
    : p1(p1), p2(p2)
{
}

Triangle::Triangle() {}

Triangle::Triangle(vec2 p1, vec2 p2, vec2 p3)
    : p1(p1), p2(p2), p3(p3)
{
}

float deg2rad(float angle)
{
    return angle * (CONST_PI / 180);
}

float rad2deg(float radians)
{
    return radians * (180 / CONST_PI);
}
