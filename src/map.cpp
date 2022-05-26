#include "map.hpp"

OuterWalls::OuterWalls() {}

void OuterWalls::setPerimeter(SDL_Rect *port, double size)
{
    double frame = (1 - size) / 2;

    // store map values
    this->x = port->w * frame;
    this->y = port->h * frame;
    this->w = port->w * (1 - frame) - this->x;
    this->h = port->h * (1 - frame) - this->y;

    // set SDL rect points
    this->recWalls.x = this->x;
    this->recWalls.y = this->y;
    this->recWalls.w = this->w;
    this->recWalls.h = this->h;
}

void OuterWalls::render(Window *w, SDL_Rect *port)
{
    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 255, 1);
    SDL_RenderDrawRect(w->renderer, &this->recWalls);
}
