#include "headers.h"

/**
 * textureInit -
 *
 *
 */
void textureInit(texture *t, unsigned int *tCount)
{
    t->texture = NULL;
    t->width = 0;
    t->height = 0;
    t->id = *tCount + 1;
    *tCount++;
}

/**
 * textureDelete -
 *
 *
 */
void textureClean(texture *t)
{
    if (t->texture != NULL)
    {
        SDL_DestroyTexture(t->texture);
        t->texture = NULL;
        t->width = 0;
        t->height = 0;
    }
}

/**
 * textureLoad -
 *
 *
 */
void textureLoad(texture *t, sdl_window *w, char *filePath)
{
    SDL_Surface *loadedSurface = IMG_Load(filePath);
    t->success = false;

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filePath, IMG_GetError());
        return;
    }

    // color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

    // create texture from suf pixels
    t->texture = SDL_CreateTextureFromSurface(w->renderer, loadedSurface);
    if (t->texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", filePath, SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return;
    }
    t->width = loadedSurface->w;
    t->height = loadedSurface->h;
    t->success = true;
}

/**
 * textureSetBlending - 
 * 
 * 
 */
void textureSetBlending(texture *t, SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(t->texture, blending);
}

/**
 * textureAlphaMod - 
 * 
 * 
 */
void textureAlphaMod(texture *t, Uint8 alpha)
{
    SDL_SetTextureAlphaMod(t->texture, alpha);
}

/**
 * textureRender - 
 *
 *
 */
void textureRender(texture *t, sdl_window *w, int x, int y)
{
    // set rendering space and render to screen
    SDL_Rect renderQuad;
    renderQuad.x = x;
    renderQuad.y = y;
    renderQuad.w = t->width;
    renderQuad.h = t->height;

    SDL_RenderCopy(w->renderer, t->texture, NULL, &renderQuad);
}
