#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>
#include <SDL_image.h>

class Window {
public:
    
    // constructor
    Window(int w, int h);
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *screenSurface;

    bool windowShouldClose;
    bool status;

    void initWindow();
    void cleanup();

    // getters
    int getWidth() { return this->scr_Width; };
    int getHeight() { return this->scr_Height; };

private:
    int scr_Width;
    int scr_Height;
};

#endif