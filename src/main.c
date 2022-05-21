#include "headers.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


void pollEvents(sdl_window *window, SDL_Event *event);

/**
* main - entry point
* Return: 0 for success
*/
int main(int argc, char **argv) 
{
    
    sdl_window window;
    media_handler media;
    bool quit = false;
    SDL_Event event;

    if (!initWindow(&window, SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        cleanUp(&window);
        printf("Exiting program as a result of failing to initialice SDL window");
        return (1);
    }

    loadMedia(&media, &window, "../assets/images/space_1.png");

    while (!window.windowShouldClose)
    {
        // handle events on queue
        pollEvents(&window, &event);

        /* Fill the surface white */
        //SDL_FillRect(window.screenSurface, NULL, SDL_MapRGB(window.screenSurface->format, 0xFF, 0xFF, 0xFF));
        SDL_BlitSurface(media.image, NULL, window.screenSurface, NULL);

        /* Update the surface */
        SDL_UpdateWindowSurface(window.window);
    }
    /* cleanup SDL */
    cleanUp(&window);

    return (0);
}

void pollEvents(sdl_window *window, SDL_Event *event)
{
    // User request
    while (SDL_PollEvent(event) != 0)
    {
        if (event->type == SDL_QUIT)
            window->windowShouldClose = true;
    }
}
