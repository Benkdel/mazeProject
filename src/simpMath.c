#include "simpMath.h"


/**
 * normalizeVec - 
 * 
 * 
 * 
 */
void normalizeVec(vec3 *v3, vec3 origin)
{
    float x = v3->x - origin.x;
    float y = v3->y - origin.y;
    float z = v3->z - origin.z;
    float magnitude;

    magnitude = sqrt(x * x + y * y + z * z);

    if (magnitude > 0)
    {
        // normalize vector:
        v3->x /= magnitude;
        v3->y /= magnitude;
        v3->z /= magnitude;
    }
}

/**
 * scalarMult - 
 * 
 * 
 * 
 */
vec3 scalarMult(vec3 vec, float value)
{
    float x, y, z;
    vec3 result;
    
    result.x = vec.x * value;
    result.y = vec.y * value;
    result.z = vec.z * value;
    
    return (result);
}

/**
 * vectorMult - 
 * 
 * 
 */
vec3 vectorMult(vec3 vec1, vec3 vec2)
{
    vec3 result;

    result.x = vec1.x * vec2.x;
    result.y = vec1.y * vec2.y;
    result.z = vec1.z * vec2.z;

    return (result);
}

/**
 * vectorAddition - 
 * 
 * 
 */
vec3 vectorAddition(vec3 vec1, vec3 vec2)
{
    vec3 result;

    result.x = vec1.x + vec2.x;
    result.y = vec1.y + vec2.y;
    result.z = vec1.z + vec2.z;

    return (result);
}

/**
 * vectorAddition - 
 * 
 * 
 */
vec3 vectorSubstraction(vec3 vec1, vec3 vec2)
{
    vec3 result;

    result.x = vec1.x - vec2.x;
    result.y = vec1.y - vec2.y;
    result.z = vec1.z - vec2.z;
    
    return (result);
}

/**
 * getVecFromAngle - 
 * 
 */
vec3 getVecFromAngle(float magnitude, float angle)
{
    vec3 result;
    float radians = angle * (CONST_PI / 180);

    result.x = acos(radians) * magnitude;
    result.y = asin(radians) * magnitude;
    result.z = 0; // for now we are working in 2D

    return result;
}

/**
 * distanceBtwPoints - 
 * 
 * 
 */
float distanceBtwPoints(vec3 p1, vec3 p2)
{
    // for now its only 2D
    float distX;
    float distY;

    distX = (p1.x - p2.x) * (p1.x - p2.x);
    distY = (p1.y - p1.y) * (p1.y - p2.y);
    
    return (sqrt(distX + distY));
}

/**
 * clampInts - 
 * 
 * 
 */
void clampInts(int *value, int lower, int upper)
{
    *value = (*value > upper) ? upper : *value;
    *value = (*value < lower) ? lower : *value;
} 
