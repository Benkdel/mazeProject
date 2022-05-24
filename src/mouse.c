#include "mouse.h"


/**
 * mouseInit - 
 * 
 */
void mouseInit(sdl_window *w, mouse_handler *m)
{
    m->position.x = w->SCREEN_WIDTH / 2;
    m->position.y = w->SCREEN_HEIGHT / 2;

    m->lastX = m->position.x;
    m->lastY = m->position.y;
}

/**
 * mouseSetPosition
 * 
 */
void mouseSetPosition(mouse_handler *m, int x, int y)
{
    m->lastX = m->position.x;
    m->lastY = m->position.y;

    m->position.x = x;
    m->position.y = y;

    //printf("Mouse x: %d , Mouse y: %d\n", m->position.x, m->position.y);
}
