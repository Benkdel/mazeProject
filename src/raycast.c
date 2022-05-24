#include "headers.h"


/**
 * rayCasting -
 * 
 * 
 */
bool rayCasting(line l1, player l2, vec3 *intPoint)
{
    float t = -1; // must be betw 0 and 1
    float u = -1; // must be > 0

    // use wiky notation
    const float x1 = l1.x1;
    const float y1 = l1.y1;
    const float x2 = l1.x2;
    const float y2 = l1.y2;
    const float x3 = l2.pos.x;
    const float y3 = l2.pos.y;
    const float x4 = l2.dir.x;
    const float y4 = l2.dir.y;

    float den;

    den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den != 0)
    {
        t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
        u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

        if (t > 0 && t < 1 && u > 0)
        {
            intPoint->x = x1 + t * (x2 - x1);
            intPoint->y = y1 + t * (y2 - y1);
            return (true);
        }
        else
        {
            return (false);

        }
    }
    return (false);
}
