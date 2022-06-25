#include <iostream>

#include "window.hpp"

Window::Window(int w, int h)
    : scr_Width(w), scr_Height(h)
{
    initWindow();
}

void Window::initWindow()
{
    int imgFlags = IMG_INIT_PNG;
    this->status = false;

    /* Initialization of SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        return;
    }

    // set window should close to false
    this->windowShouldClose = false;

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        std::cout << "Warning: Linear texture filtering not enabled!\n";
    }

    this->window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->scr_Width, this->scr_Height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
        return;
    }

    // Create renderer for window
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if (this->renderer == NULL)
    {
        std::cout << "Renderer could not be created! SDL Error: " <<  SDL_GetError() << "\n";
        return;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: \n" << IMG_GetError() << "\n";
        return;
    }

    this->screenSurface = SDL_GetWindowSurface(this->window);

    this->status = true;
}

void Window::cleanup()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    this->renderer = NULL;
    this->window = NULL;
    this->screenSurface = NULL;
}
