#ifndef SIMPMATH_H
#define SIMPMATH_H

/* includes */
#include <math.h>


/* macros */
#define CONST_PI 3.14159265358979323846


/* structs */

/**
 * struct vector3D - 
 * 
 */
typedef struct vector3D {
    float x;
    float y;
    float z;
} vec3;


/* prototypes */
void normalizeVec(vec3 *v3, vec3 origin);


/* vector arithmetic */
vec3 scalarMult(vec3 vec, float value);
vec3 vectorMult(vec3 vec1, vec3 vec2);
vec3 vectorAddition(vec3 vec1, vec3 vec2);

/* trigonometry / Geometry functions */
vec3 getVecFromAngle(float magnitude, float angle);
float distanceBtwPoints(vec3 p1, vec3 p2);

#endif