#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../math/myMathLib.hpp"
#include <SDL.h>
#include "texture.hpp"

#include <stdio.h>
#include <string.h>

namespace pixColors {
    struct rgb {
        Uint32 r;
        Uint32 g;
        Uint32 b;
    };

    const Uint32 red = 0xFF0000;
    const Uint32 white = 0xFFFFFF;
    const Uint32 black = 0x000000;
    const Uint32 crimson = 0xDC143C;
    const Uint32 fireBrick = 0xB22222;
    const Uint32 darkSalmon = 0xE9967A;
    const Uint32 gold = 0xFFD700;
    const Uint32 yellow = 0xFFFF00;
    const Uint32 magenta = 0xFF00FF;
    const Uint32 purple = 0x800080;
    const Uint32 stateBlue = 0x6A5ACD;
    const Uint32 steelBlue = 0x4682B4;
    const Uint32 Cyan = 0x00FFFF;
};

class Sprite
{
public:
    Sprite() {};

    Sprite(Texture *tex)
    {
        load(tex);
    }

    Sprite(int w, int h)
    : nWidth(w), nHeight(h) {
        this->create(w, h);
    }

    int nWidth = 0;
    int nHeight = 0;
    
    void setColor(int x, int y, Uint32 c)
    {
        if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
            return;
        mColors[y * nWidth + x] = c;
    }

    Uint32 GetColor(int x, int y)
    {
        if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
            return pixColors::black;
        return mColors[y * nWidth + x];
    }

    pixColors::rgb GetRGBColor(int x, int y)
    {     
        return hexToRgb(mColors[y * nWidth + x]);
    }

    Uint32 SampleColor(float x, float y)
    {
        int sx = (int)(x * (float)nWidth);
        int sy = (int)(y * (float)nHeight-1.0f);
        if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
            return pixColors::black;
        return mColors[sy * nWidth + sx];
    }

    pixColors::rgb SampleRGBColor(float x, float y)
    {
        struct pixColors::rgb rgbColor;
        
        int sx = (int)(x * (float)nWidth);
        int sy = (int)(y * (float)nHeight-1.0f);
        if (sx < 0 || sx >= nWidth || sy < 0 || sy >= nHeight)
            return rgbColor = {pixColors::black};

        return GetRGBColor(sx, sy);
    }


    // utils

    Uint32 rgbToHex(int r, int g, int b)
    {
        return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    }

    pixColors::rgb hexToRgb(Uint32 hex)
    {
        struct pixColors::rgb rgbColor;

        rgbColor.r = hex >> 16;
        rgbColor.g = ((hex >> 8) & 0xFF);
        rgbColor.b = (hex & 0xFF);

        return rgbColor;
    }

private:
    Uint32 *mColors = nullptr;
    int mPitch;

    void create(int w, int h)
    {
        nWidth = w;
        nHeight = h;
        mColors = new unsigned int[w*h];
        for(int i = 0; i < w*h; i++)
            mColors[i] = pixColors::black;
    }

    void load(Texture *tex)
    {
        delete[] mColors;
        
        tex->lockTexture();
        
        Uint32 *pixels = (Uint32*)tex->getPixels();

        mPitch = tex->getPitch();
        nWidth = mPitch / 4;
        nHeight = tex->getHeight();
        int pixelCount = nWidth * nHeight;

        mColors = new Uint32[pixelCount];
        // get colors from pixels
        for (int p = 0; p < pixelCount; p++)
        {
            // get pixel color:
            Uint8 r, g, b;
            SDL_GetRGB(pixels[p], tex->getPixFormat(), &r, &g, &b);
            
            mColors[p] = rgbToHex(r, g, b);
        }
        tex->unlockTexture();        
    }
};


#endif