#ifndef TESTS_HPP
#define TESTS_HPP

#include "../sdl_engine/window.hpp"
#include "../math/myMathLib.hpp"
#include "../worlds/map.hpp"
#include "../io/keyboard.hpp"
#include "../io/mouse.hpp"
#include "../shapes/circle.hpp"

#include <stdio.h>
#include <string.h>

#define mapSize 1024

namespace RGB {
    struct rgb {
        unsigned int r;
        unsigned int g;
        unsigned int b;
    };

    const unsigned int red = 0xFF0000;
    const unsigned int white = 0xFFFFFF;
    const unsigned int black = 0x000000;
    const unsigned int crimson = 0xDC143C;
    const unsigned int fireBrick = 0xB22222;
    const unsigned int darkSalmon = 0xE9967A;
    const unsigned int gold = 0xFFD700;
    const unsigned int yellow = 0xFFFF00;
    const unsigned int magenta = 0xFF00FF;
    const unsigned int purple = 0x800080;
    const unsigned int stateBlue = 0x6A5ACD;
    const unsigned int steelBlue = 0x4682B4;
    const unsigned int Cyan = 0x00FFFF;
};

class Sprite
{
public:
    Sprite() {};

    Sprite(Window *w, const char *filePath)
    {
        load(w, filePath);
    }

    Sprite(int w, int h)
    : nWidth(w), nHeight(h) {
        this->create(w, h);
    }

    int nWidth = 0;
    int nHeight = 0;
    
    void setColor(int x, int y, unsigned int c)
    {
        if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
            return;
        mColors[y * nWidth + x] = c;
    }

    unsigned int GetColor(int x, int y)
    {
        if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
            return RGB::black;
        return mColors[y * nWidth + x];
    }

    RGB::rgb GetRGBColor(int x, int y)
    {
        struct RGB::rgb rgbColor;

        if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
            return rgbColor = {RGB::black};
        
        int hexColor = mColors[y * nWidth + x];
        rgbColor.r = hexColor >> 16;
        rgbColor.g = ((hexColor >> 8) & 0xFF);
        rgbColor.b = (hexColor & 0xFF);

        return rgbColor;
    }

    unsigned int SampleColor(float x, float y)
    {
        int sx = (int)(x * (float)nWidth);
        int sy = (int)(y * (float)nHeight-1.0f);
        if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
            return RGB::black;
        return mColors[sy * nWidth + sx];
    }

    RGB::rgb SampleRGBColor(float x, float y)
    {
        struct RGB::rgb rgbColor;
        
        int sx = (int)(x * (float)nWidth);
        int sy = (int)(y * (float)nHeight-1.0f);
        if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
            return rgbColor = {RGB::black};

        return GetRGBColor(sx, sy);
    }

private:
    unsigned int *mColors = nullptr;
    int mPitch;
    void *mPixels;

    void create(int w, int h)
    {
        nWidth = w;
        nHeight = h;
        mColors = new unsigned int[w*h];
        for(int i = 0; i < w*h; i++)
            mColors[i] = RGB::black;
    }

    void load(Window *w, const char *filePath)
    {
        delete[] mColors;
        
        SDL_Texture *newTexture = NULL;
        SDL_Surface *loadedSurface = IMG_Load(filePath);                                                      if (loadedSurface == NULL) std::cout << SDL_GetError() << "\n";
        SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0); if (formattedSurface == NULL) std::cout << SDL_GetError() << "\n";
        newTexture = SDL_CreateTexture(w->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
                    formattedSurface->w, formattedSurface->h);                                                if (newTexture == NULL) std::cout << SDL_GetError() << "\n";

        SDL_LockTexture(newTexture, NULL, &mPixels, &nWidth);
        memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

        nWidth = mPitch / 4;
        nHeight = formattedSurface->h;
        
        // load pixels
        this->mColors = (unsigned int*)this->mPixels;
        
        SDL_UnlockTexture(newTexture);
        SDL_FreeSurface(formattedSurface);
        SDL_FreeSurface(loadedSurface);
    }
};


class TestsModule {
public:

    TestsModule(Window *window, Mouse *mouse, Keyboard *keyboard);

    void setPort();

    void setMapPerimeter();
    
    void updatePos(float dt);
    void updateAngle(float dt);
    void render(float dt);

    void debugging();

    void cleanup();

    vec2 centerPoint;

    Sprite *mGround;

private:
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;

    SDL_Rect VPscreen;

    int scr_W, scr_H;

    int pixelW = 4;
    int pixelH = 4;

    float fWorldX = 1000.0f;
    float fWorldY = 1000.0f;
    float fWorldA = 0.0f;

    float fNear = 0.01f;
    float fFar = 0.1f;

    float fFovHalf = CONST_PI / 4.0f;
};



#endif