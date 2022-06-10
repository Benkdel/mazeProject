#include "player.hpp"

#include "../shapes/circle.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cmath>

/**
 * draw triangle
 *
 *
 */
void drawTriangle(Window *w, Triangle t)
{
    SDL_RenderDrawLineF(w->renderer, t.p2.x, t.p2.y, t.p3.x, t.p3.y); // nose to right leg
    SDL_RenderDrawLineF(w->renderer, t.p3.x, t.p3.y, t.p1.x, t.p1.y); // right to left leg
    SDL_RenderDrawLineF(w->renderer, t.p1.x, t.p1.y, t.p2.x, t.p2.y); // left to nose
}

Player::Player() {}

void Player::init(Map *w)
{
    this->pos = vec2(w->w / 2, w->h / 2);

    this->angle = 0.0f; // because we start with the triangle pointing upwards

    this->radius = 6.0f * CONST_PI;

    // left leg, nose, and right leg (points pointing to the right at 0 degrees) // y is inverted
    this->triangle = Triangle(vec2(0.0f, -0.5f), vec2(0.5f, 0.0f), vec2(0.0f, 0.5f));
}

void Player::updateCurrentAngle(Keyboard *kb, float dt)
{
    // update angle correctly
    float rotation = 50.0f * kb->angle * dt;

    // reset keyboard angle
    kb->angle = 0.0f;

    this->angle += rotation;

    if (this->angle < 0.0f)
        this->angle = 360.0f + this->angle;
    if (this->angle > 360.0f)
        this->angle = this->angle - 360.0f + 0.00000001f;
}

void Player::updatePos(Keyboard *kb, float dt)
{

    this->pos.x += cosf(deg2rad(this->angle)) * kb->acceleration * 5.0f * dt;
    this->pos.y += -sinf(deg2rad(this->angle)) * kb->acceleration * 5.0f * dt;

    kb->acceleration = 0.0f;

    // update box collider
    this->box_collider.x = this->pos.x - this->radius * 7 / 10;
    this->box_collider.y = this->pos.y - this->radius * 7 / 10;
    this->box_collider.w = this->radius * 14 / 10;
    this->box_collider.h = this->radius * 14 / 10;
}

void Player::render(Window *w, SDL_Rect *port)
{
    // draw box collider
    // SDL_SetRenderDrawColor(w->renderer, 247, 255, 254, 1);
    // SDL_RenderFillRect(w->renderer, &this->box_collider);

    // draw player
    SDL_RenderSetViewport(w->renderer, port);
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 51, 255);

    // transformations
    Triangle transTriangle;

    // scale
    float size = 40.0f;
    transTriangle.p1 = scale2Dvec(this->triangle.p1, size);
    transTriangle.p2 = scale2Dvec(this->triangle.p2, size);
    transTriangle.p3 = scale2Dvec(this->triangle.p3, size);

    // rotate
    // im negating the angles to flip directions correclty
    transTriangle.p1 = rotate2Dvec(transTriangle.p1, -this->angle);
    transTriangle.p2 = rotate2Dvec(transTriangle.p2, -this->angle);
    transTriangle.p3 = rotate2Dvec(transTriangle.p3, -this->angle);

    // translate
    transTriangle.p1 = transTriangle.p1 + this->pos;
    transTriangle.p2 = transTriangle.p2 + this->pos;
    transTriangle.p3 = transTriangle.p3 + this->pos;

    // reset rays
    for (int i = 0; i < MAX_RAYS; i++)
        this->rays[i].rayDir = vec2(0.0f, 0.0f);

    // set FoV vectors or Rays
    this->lookAt = transTriangle.p2;
    
    float fAngle = this->angle - (FOV / 2);
    if (fAngle < 0.0f)
        fAngle = 360.0f + fAngle;
    if (fAngle > 360.0f)
        fAngle = fAngle - 360.0f + 0.0001f;

    for (int i = 0; i < MAX_RAYS; i++)
    {
        this->rays[i].rayDir = getVecFromAngle(1.0f, fAngle + (float)i);
    }

    // draw triangle
    drawTriangle(w, transTriangle);

    // draw line from player to triangle
    SDL_RenderDrawLineF(w->renderer, (float)this->pos.x, (float)this->pos.y, transTriangle.p2.x, transTriangle.p2.y);
}

void Player::rayCastDDD(Window *window, SDL_Rect *port, Map *map)
{
    HitResult hitResult;
    for (int i = 0; i < MAX_RAYS; i++)
    {
        hitResult = this->rays[i].castDDD(this->pos, this->rays[i].rayDir, map);

        // then draw closest hit
        SDL_RenderSetViewport(window->renderer, port);
        if (hitResult.hit)
            SDL_SetRenderDrawColor(window->renderer, 153, 0, 76, 255);
        else
            SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(window->renderer, this->pos.x, this->pos.y, hitResult.intersection.x, hitResult.intersection.y);
    }
}

void Player::clearRays()
{
    //
}
