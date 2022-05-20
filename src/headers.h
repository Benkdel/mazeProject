#ifndef HEADERS_H
#define HEADERS_H

/* includes */

/* libs */
#include <SDL.h>

/* std library */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* structs */

/**
* struct sdl_window - stores window instance and renderer
* elaborate on variables
* 
*/
typedef struct sdl_window {
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    bool windowShouldClose;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screenSurface;

} sdl_window;

/* prototypes */
bool initWindow(sdl_window* window, int scr_w, int scr_h);
void cleanUp(sdl_window* window);

#endif