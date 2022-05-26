#include "myMathLib.hpp"

#include <cmath>

vec2::vec2(){}

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

vec2 vec2::operator*(vec2 const &obj)
{
    vec2 res;
    res.x = x * obj.x;
    res.y = y * obj.y;
    return res;
}

 void normalizeVec(vec2 *vec, vec2 origin)
{
    float x = vec->x - origin.x;
    float y = vec->y - origin.y;
    float magnitude;

    magnitude = sqrt(x * x + y * y);

    if (magnitude > 0)
    {
        // normalize vector:
        vec->x /= magnitude;
        vec->y /= magnitude;
    }
}

 vec2 getVecFromAngle(float magnitude, float angle)
{
    vec2 result;
    float radians = angle * (CONST_PI / 180);

    result.x = acos(radians) * magnitude;
    result.y = asin(radians) * magnitude;

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