#ifndef HEADERS_H
#define HEADERS_H

/* ========================================== */
/* INCLUDES */
/* ========================================== */

/* libs */
#include <SDL.h>
#include <SDL_image.h>

/* my h files */
#include "mouse.h"
#include "keyboard.h"
#include "simpMath.h"

/* std library */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========================================== */
/* MACROS */
/* ========================================== */
#define MAX_RAYS 36
#define MAX_WALLS 10

/* ========================================== */
/* ENUMS */
/* ========================================== */

/* ======================================= */
/* STRUCTS */
/* ======================================= */

/**
 * struct sdl_window - stores window instance and renderer
 * @SCREEN_WIDTH:
 * @SCREEN_HEIGHT:
 * @windowShouldClose:
 * @window:
 * @renderer:
 * @screenSurface:
 */
typedef struct sdl_window
{
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    bool windowShouldClose;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *screenSurface;

} sdl_window;

/**
 * struct texture - handles textures
 * @texture: SDL object handler for textures
 * @id: id of texture
 * @width: width of image
 * @height: height of image
 */
typedef struct texture_class
{
    SDL_Texture *texture;
    int id;
    int width;
    int height;
    bool success;
} texture;


/* type vectors */
// 

/**
 * struct line - 
 * 
 */
typedef struct line {
    unsigned int x1;
    unsigned int x2;
    unsigned int y1;
    unsigned int y2;
} line;

/* WORLD STRUCTS */

/**
 * struct outer_walls - 
 * 
 * 
 */
typedef struct outer_walls
{
    SDL_Rect sdl_walls;

    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;

    double sizeOfMap;
} outer_walls;

/* entities */

/**
 * struct entity_player -
 * 
 * 
 * 
 */
typedef struct entity_player {
    vec3 pos;
    vec3 dir; // should be a normalize vector
    float fov; // field of view
    vec3 rays[MAX_RAYS];
    unsigned int noRays;

} player;


/* ========================================== */
/* PROTOTYPES */
/* ========================================== */

bool initWindow(sdl_window *, int, int);
void cleanUpWindow(sdl_window *);

/* map functions */
void setOuterWalls(sdl_window *, outer_walls *, double);
void renderOuterWalls(sdl_window *w, outer_walls *walls);
void renderWall(sdl_window *, line *);

/* texture methods prototypes */
void textureInit(struct texture_class *, unsigned int *);
void textureClean(struct texture_class *);
void textureLoad(struct texture_class *, sdl_window *w, char *);
void textureRender(struct texture_class *, sdl_window *w, int, int);
void textureSetBlending(texture *t, SDL_BlendMode blending);
void textureAlphaMod(texture *t, Uint8 alpha);

/* math - should be in another library */
bool rayCasting(line l1, player l2, vec3 *intPoint);

/* player methods */
void playerInit(player *p, vec3 pos, vec3 dir, float fov);
void playerUpdateDir(player *p, vec3 dir);
void playerUpdatePos(player *p, vec3 pos);
void playerRender(sdl_window *w, player *p);

#endif
