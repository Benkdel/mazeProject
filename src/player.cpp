#include "player.hpp"

#include "circle.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>

Player::Player() {}

void Player::init(Map *w)
{
    this->pos = vec2(w->w / 2, w->h / 2);
    this->dir = this->pos + vec2(100, 0);
    std::vector<Ray> vec(MAX_RAYS, Ray());
    this->rays = vec;
}

void Player::updateDir(vec2 dir)
{
    this->dir = dir;
}

void Player::updatePos(vec2 pos)
{
    float velocity = 1.05f;
    this->pos = pos * vec2(velocity, velocity);
}

void Player::render(Window *w, SDL_Rect *port)
{
    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 51, 1);
    Circle c;
    c.drawCircle(w->renderer, (int32_t)this->pos.x, (int32_t)this->pos.y, CONST_PI * 2);
}

void Player::setUpRays()
{
    int i = 0;
    std::vector<Ray>::iterator it = this->rays.begin();
    std::vector<Ray>::iterator end = this->rays.end();

    for (it; it != end; ++it)
    {
        it->pos = this->pos;
        it->dir = this->dir + getVecFromAngle(100, i);
        i++;
    }
}

void Player::clearRays()
{
    this->rays.clear();
}
