#include "texture.hpp"

#include <iostream>
#include <string.h>

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

    // The final texture
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(filePath);
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image " << filePath << " SDL_image Error: " << IMG_GetError();
        return;
    }

    // convert surface to display format
    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(w->window), 0);
    if (formattedSurface == NULL)
    {
        std::cout << "Unable to convert loaded surface to display format SDL_image Error: " << SDL_GetError();
        return;
    }

    // create texture from suf pixels
    newTexture = SDL_CreateTexture(w->renderer, SDL_GetWindowPixelFormat(w->window), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
    if (newTexture == NULL)
    {
        std::cout << "Unable to create blank texture SDL Error: " << SDL_GetError();
        return;
    }

    // lock texture for manipulation
    SDL_LockTexture(newTexture, NULL, &this->pixels, &this->pitch);

    // copy loaded/formatted surface pixels
    memcpy(this->pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

    // Unlock texture to update
    SDL_UnlockTexture(newTexture);
    this->pixels = NULL;

    this->w = formattedSurface->w;
    this->h = formattedSurface->h;
    this->status = true;
    this->tex = newTexture;

    // clean memory
    SDL_FreeSurface(formattedSurface);
    SDL_FreeSurface(loadedSurface);
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

bool Texture::lockTexture()
{
    if (this->pixels != NULL)
    {
        std::cout << "Texture is already locked \n";
        return false;
    }
    else
    {
        if (SDL_LockTexture(this->tex, NULL, &this->pixels, &this->pitch) != 0)
        {
            std::cout << "Unable to lock textures " << SDL_GetError() << "\n";
            return false;
        }
    }
    return true;
}

bool Texture::unlockTexture()
{
    if (this->pixels == NULL)
    {
        std::cout << "Texture is not locked \n";
        return false;
    }
    else
    {
        SDL_UnlockTexture(this->tex);
        this->pixels = NULL;
        this->pitch = 0;
    }
}
