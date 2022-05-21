#include "headers.h"

/**
 * loadMedia - loads an image from path
 *
 *
 */
void loadMedia(media_handler *mh, sdl_window *w, char *imgPath)
{
    // load image
    SDL_Surface *loadedSurface = IMG_Load(imgPath);

    mh->image = NULL;
    mh->success = false;

    if (loadedSurface == NULL)
    {
        printf("Unable to load iamge %s! SDL_image Error: %s\n", imgPath, IMG_GetError());
        return;
    }

    mh->image = SDL_ConvertSurface(loadedSurface, w->screenSurface->format, 0);
    if (mh->image == NULL)
    {
        printf("Unable to optimize image %s! SDL Error: %s\n", imgPath, SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return;
    }

    mh->success = true;
}
