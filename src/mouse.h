#ifndef MOUSE_H
#define MOUSE_H

/* includes */
#include "headers.h"

/* structs */

typedef struct mouse_handler {
    SDL_Point position;

    int lastX;
    int lastY;

} mouse_handler;


/* prototypes */
void mouseSetPosition(mouse_handler *m, int x, int y);



#endif