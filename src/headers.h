#ifndef HEADERS_H
#define HEADERS_H

/* includes */

/* libs */
#include <SDL.h>
#include <SDL_image.h>

/* std library */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* structs */

/**
* struct sdl_window - stores window instance and renderer
* @SCREEN_WIDTH:
* @SCREEN_HEIGHT:
* @windowShouldClose:
* @window:
* @renderer:
* @screenSurface:
*/
typedef struct sdl_window {
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    bool windowShouldClose;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screenSurface;

} sdl_window;

/**
 * struct media_handler - stores and iamge load from path
 * @image: SDL_Surface pointer to image loaded
 * @success: indicates if loading succeded or failed
 */
typedef struct media_handler {
    SDL_Surface *image;
    bool success;
} media_handler;

/* prototypes */
bool initWindow(sdl_window* window, int scr_w, int scr_h);
void cleanUp(sdl_window* window);
void loadMedia(media_handler *mh, sdl_window *w, char *imgPath);

#endif