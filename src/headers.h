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
#define MAX_RAYS 90
#define MAX_WALLS 10
#define MAX_SCENE_WIDTH 1024

/* ========================================== */
/* ENUMS */
/* ========================================== */


/* ========================================== */
/* GLOBAL VARIABLES */
/* ========================================== */
bool printData;


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

/**
 *  
 * 
 */
typedef struct inner_walls {
    line l[MAX_WALLS];
} inner_walls;

/* entities */

/**
 * ray - 
 * 
 * 
 */
typedef struct ray {
    vec3 pos;
    vec3 dir;
} ray;

/**
 * struct entity_player -
 * 
 * 
 * 
 */
typedef struct entity_player {
    vec3 pos;
    vec3 dir; // should be a normalize vector
    ray rays[MAX_RAYS];
    unsigned int noRays;
    texture text;

} player;


/* ========================================== */
/* PROTOTYPES */
/* ========================================== */

bool initWindow(sdl_window *, int, int);
void cleanUpWindow(sdl_window *);

/* map functions */
void setOuterWalls(outer_walls *, SDL_Rect *, double);
void renderOuterWalls(const sdl_window *w, const outer_walls *, const SDL_Rect *);
void setInnerWalls(inner_walls *, const outer_walls *);
void renderInnerWalls(const sdl_window *, const inner_walls *, const SDL_Rect *);
void setMinimapPort(SDL_Rect *, unsigned int, unsigned int);
void setWorldPort(SDL_Rect *, unsigned int, unsigned int);

/* texture methods prototypes */
void textureInit(struct texture_class *);
void textureClean(struct texture_class *);
void textureLoad(struct texture_class *, sdl_window *, char *);
void textureRender(struct texture_class *, sdl_window *, int, int);
void textureSetBlending(texture *, SDL_BlendMode);
void textureAlphaMod(texture *, Uint8);

/* math - should be in another library */
bool rayCasting(line, ray, vec3 *);

/* player methods */
void playerInit(sdl_window *, player *, vec3, vec3, float);
void playerUpdateDir(player *, vec3);
void playerUpdatePos(player *, vec3);
void playerRender(sdl_window *, player *, SDL_Rect *);

/* SCENE METHODS */
void renderScene(sdl_window *, SDL_Rect *, const line *, size_t);

/* SHAPES */
void drawCircle(SDL_Renderer *, int32_t, int32_t, int32_t);

#endif
