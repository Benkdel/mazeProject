#ifndef TETURE_HPP
#define TEXTURE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include "window.hpp"

class Texture {
public:
    
    // Empty constructor
    Texture();

    // destructor
    ~Texture();

    void load(Window *w, const char *filePath);
    void setBlending(SDL_BlendMode blending);
    void alphaMod(Uint8 alpha);
    void render(Window *w, int x, int y);
    void cleanup();

private:
    SDL_Texture *tex;

    int x;
    int y;
    int w;
    int h;

    bool status;
};


#endif