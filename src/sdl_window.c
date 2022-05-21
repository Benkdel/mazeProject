#include "headers.h"

/**
* initWindows
*
* Return: true if windows init, false if not
*/
bool initWindow(sdl_window* window, int scr_w, int scr_h)
{
    int imgFlags = IMG_INIT_PNG;
    
    /* Initialization of SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return (false);
    }

    // set window should close to false
    window->windowShouldClose = false;

    //Set texture filtering to linear
	if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
	{
			printf( "Warning: Linear texture filtering not enabled!" );
	}

    window->window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr_w, scr_h, SDL_WINDOW_SHOWN);
    if(window == NULL)
	{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return (false);
    }

    //Create renderer for window
	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if(window->renderer == NULL)
	{
	    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return (false);
	}

    //Initialize renderer color
    SDL_SetRenderDrawColor(window->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return (false);
    }

    window->screenSurface = SDL_GetWindowSurface(window->window);

    return (true);
}

/**
 * cleanup - ...
 *
 * 
 */
void cleanUp(sdl_window* window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    window->renderer = NULL;
    window->window = NULL;
    window->screenSurface = NULL;

    // IMG_Quit();
    SDL_Quit();
}