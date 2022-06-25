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

Texture::Texture(Window *w, const char *filePath, int loadType)
{
    if (loadType == 0)
        this->load(w, filePath);
    if (loadType == 1)
        this->simpleLoad(w, filePath);
}

Texture::~Texture()
{
    this->cleanup();
}

void Texture::simpleLoad(Window *w, const char *filepath)
{
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(filepath);
    if (loadedSurface == NULL)
    {
        std::cout << "Unable to load image " << filepath << " SDL_image Error: " << IMG_GetError() << "\n";
    }
    else
    {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(w->renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cout << "Unable to create texture from " << filepath << " SDL Error: " << SDL_GetError() << "\n";
        }
        else
        {
            // Get image dimensions
            this->w = loadedSurface->w;
            this->h = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    this->tex = newTexture;
}

void Texture::load(Window *w, const char *filePath)
{
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
    newTexture = SDL_CreateTexture(w->renderer, SDL_GetWindowPixelFormat(w->window) /*SDL_PIXELFORMAT_RGBA8888*/, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
    if (newTexture == NULL)
    {
        std::cout << "Unable to create blank texture SDL Error: " << SDL_GetError();
        return;
    }

    // lock texture for manipulation
    SDL_LockTexture(newTexture, NULL, &this->pixels, &this->pitch);

    // copy loaded/formatted surface pixels
    memcpy(this->pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

    this->w = formattedSurface->w;
    this->h = formattedSurface->h;

    // Get pixel data in editable format
    Uint32 *pixels = (Uint32 *)this->pixels;

    int pixelCount = (this->pitch / 4) * this->h;
    // Map colors
    Uint32 colorKey = SDL_MapRGB(formattedSurface->format, 0, 0xFF, 0xFF);
    Uint32 transparent = SDL_MapRGBA(formattedSurface->format, 0x00, 0xFF, 0xFF, 0x00);
    // Color key pixels
    for (int i = 0; i < pixelCount; ++i)
    {
        if (pixels[i] == colorKey)
        {
            pixels[i] = transparent;
        }
    }

    // // Unlock texture to update
    SDL_UnlockTexture(newTexture);
    this->pixels = NULL;

    this->status = true;
    this->tex = newTexture;
    this->pixFormat = formattedSurface->format;

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

void Texture::render(Window *w, int x, int y, int width, int height, SDL_Rect *part)
{
    // set rendering space and render to screen
    this->dimensions.x = x;
    this->dimensions.y = y;
    this->dimensions.w = (width == 0) ? this->w : width;
    this->dimensions.h = (height == 0) ? this->h : height;

    SDL_RenderCopy(w->renderer, this->tex, part, &this->dimensions);
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
