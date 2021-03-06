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
    Texture(Window *w, const char *filePath, int loadType);

    // destructor
    ~Texture();

    void load(Window *w, const char *filePath);
    void simpleLoad(Window *w, const char *filePath);

    void setBlending(SDL_BlendMode blending);
    void alphaMod(Uint8 alpha);
    void render(Window *w, int x, int y, int witdh = 0, int height = 0, SDL_Rect *part = NULL);
    void cleanup();

    // Pixel manipulators
    bool lockTexture();
    bool unlockTexture();

    inline SDL_Texture *getSDLTexture() { return this->tex; }
    inline int getWidth() { return this->w; }
    inline int getHeight() { return this->h; }
    inline void *getPixels() { return this->pixels; }
    inline int getPitch() { return this->pitch; }
    inline SDL_PixelFormat *getPixFormat() { return this->pixFormat; }

private:
    SDL_Texture *tex;
    SDL_Rect dimensions;

    SDL_PixelFormat *pixFormat;

    void *pixels;
    int pitch;

    int x;
    int y;
    int w;
    int h;

    bool status;
};


#endif