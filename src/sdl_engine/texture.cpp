#include "texture.hpp"
#include <iostream>

Texture::Texture() 
{
    this->tex = NULL;
    this->w = 0;
    this->h = 0;
    this->status = false;
}

Texture::~Texture()
{
    this->cleanup();
}

void Texture::load(Window *w, const char *filePath)
{
    // free prev texture
    this->cleanup();
    
    SDL_Surface *loadedSurface = IMG_Load(filePath);

    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image " << filePath << " SDL_image Error: " << IMG_GetError();
        return;
    }

    // color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

    // create texture from suf pixels
    this->tex = SDL_CreateTextureFromSurface(w->renderer, loadedSurface);
    if (this->tex == NULL)
    {
        std::cout << "Unable to create texture from " << filePath << " SDL Error: " << SDL_GetError();
        SDL_FreeSurface(loadedSurface);
        return;
    }

    this->w = loadedSurface->w;
    this->h = loadedSurface->h;
    this->status = true;
}

void Texture::cleanup()
{
    if (this->tex != NULL)
    {
        SDL_DestroyTexture(this->tex);
        this->tex = NULL;
        this->w = 0;
        this->h = 0;
    }
}

void Texture::setBlending(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(this->tex, blending);
}

void Texture::alphaMod(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(this->tex, alpha);
}

void Texture::render(Window *w, int x, int y)
{
    // set rendering space and render to screen
    SDL_Rect renderQuad;
    renderQuad.x = x;
    renderQuad.y = y;
    renderQuad.w = this->w;
    renderQuad.h = this->h;

    SDL_RenderCopy(w->renderer, this->tex, NULL, &renderQuad);
}
