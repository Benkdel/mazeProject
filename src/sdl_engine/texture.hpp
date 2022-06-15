#ifndef TETURE_HPP
#define TEXTURE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include "window.hpp"

class Texture {
public:
    
    // Empty constructor
    Texture();

    // Constructor
    Texture(Window *w, const char *filePath);

    // destructor
    ~Texture();

    void load(Window *w, const char *filePath);
    void setBlending(SDL_BlendMode blending);
    void alphaMod(Uint8 alpha);
    void render(Window *w, int x, int y, int witdh = 0, int height = 0);
    void cleanup();

    // Pixel manipulators
    bool lockTexture();
    bool unlockTexture();
    
    inline int getWidth() { return this->w; }
    inline int getHeight() { return this->h; }
    inline void *getPixels() { return this->pixels; }
    inline int getPitch() { return this->pitch; }

private:
    SDL_Texture *tex;

    void *pixels;
    int pitch;

    int x;
    int y;
    int w;
    int h;

    bool status;
};


#endif