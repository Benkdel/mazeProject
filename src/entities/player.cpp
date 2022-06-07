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
    SDL_RenderDrawLineF(w->renderer, t.p1.x, t.p1.y, t.p2.x, t.p2.y); // p1 to p2
    SDL_RenderDrawLineF(w->renderer, t.p2.x, t.p2.y, t.p3.x, t.p3.y); // p2 to p3
    SDL_RenderDrawLineF(w->renderer, t.p3.x, t.p3.y, t.p1.x, t.p1.y); // p3 to p1
}

Player::Player() {}

void Player::init(Map *w)
{
    this->pos = vec2(w->w / 2, w->h / 2);
    this->velocity = vec2(0.0f, 0.0f);
    this->lookAt = vec2(this->pos.x + 1.0f, this->pos.y + 1.0f);

    this->velocity_mag = 0.0f;
    this->prevVelocity_mag = 0.0f;

    this->horizontalMaxSpeed = 100.0f;
    this->verticalMaxSpeed = 100.0f;
    this->slowdownRate = 0.3f; // times delta time and final push should be 2f?

    this->angle = 0.0f;
    this->radius = 6.0f * CONST_PI;

    this->triangle = Triangle(vec2(-2.5f, 2.5f), vec2(0.0f, -5.0f), vec2(2.5f, 2.5f));
}

void Player::updateCurrentAngle(float rotAngle)
{
    float totalRot = this->angle + rotAngle; // - because rot its applied clockwise
    if (totalRot < 0)
        totalRot = 360 - totalRot;
    if (totalRot > 360)
        totalRot = totalRot - 360 + 0.01f;
    
    this->angle = totalRot;
}

void Player::updatePos(float acceleration, vec2 mousePos, float dt, SDL_Rect *port)
{
    // update velocity and store current angle
    // gettin angle from hre is not working
    // this->angle = getAngleFromVectors(this->pos, mousePos, vec2(0.0f, 0.0f));

    // accelerate up to max speed, then mantain it until stop pusing key
    if (acceleration != 0.0f)
    {
        // im still accelerating or pushing the breaks
        if (this->prevVelocity_mag + acceleration >= this->horizontalMaxSpeed)
            this->velocity_mag = horizontalMaxSpeed;
        else if (this->prevVelocity_mag + acceleration <= 0.0f)
            this->velocity_mag = 0.0f;
        else
            this->velocity_mag += this->prevVelocity_mag + acceleration;
    }
    else
    {
        // stop accelerating
        this->velocity_mag = this->velocity_mag * (this->slowdownRate);
        if (this->velocity_mag <= 0.005f)
            this->velocity_mag = 0.0f; // final push
    }

    // store last velocity magnitud:
    this->prevVelocity_mag = this->velocity_mag;

    this->velocity.x = cosf(deg2rad(angle)) * this->velocity_mag;
    this->velocity.y = -sinf(deg2rad(angle)) * this->velocity_mag; // screen is upside down, y0 up, y last down

    if (this->velocity.x != 0)
        this->lastVelocity.x = this->velocity.x;
    if (this->velocity.y != 0)
        this->lastVelocity.y = this->velocity.y; // for debuggin

    float nextHorPos = this->pos.x + this->velocity.x * dt;
    float nextVerPos = this->pos.y + this->velocity.y * dt;

    // update position and clamp it to the screen
    float limitX = (float)(port->x + port->w) - 25.0f;
    float limitY = (float)(port->y + port->h) - 50.0f;

    if (nextHorPos <= 0.0f || nextHorPos >= limitX)
        this->velocity.x = 0.0f;
    if (nextVerPos >= limitY || nextVerPos <= 0.0f)
        this->velocity.y = 0.0f;

    this->pos.x += velocity.x * dt;
    this->pos.y += velocity.y * dt;

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
    /*Circle c;
    int32_t centerX = (int32_t)(this->pos.x /*+ CELL_SIZE / 2*/
    //);
    /*int32_t centerY = (int32_t)(this->pos.y /*+ CELL_SIZE / 2*/ //);
    /*c.drawCircle(w->renderer, centerX, centerY, this->radius);*/

    // transformations
    Triangle transTriangle;

    // scale
    float size = 5.0f;
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

    this->translTriangle = transTriangle; // just to debug

    // draw triangle
    drawTriangle(w, transTriangle);


    // update lookAt
    this->lookAt = transTriangle.p2;

    // draw line from player to triangle
    SDL_RenderDrawLineF(w->renderer, (float)this->pos.x, (float)this->pos.y, transTriangle.p2.x, transTriangle.p2.y);
}

void Player::setUpRays()
{   
    for (int i = 0; i < MAX_RAYS; i++)
    {
        this->rays[i].pos = this->pos;
        this->rays[i].dir = getVecFromAngle(1, this->angle) + getVecFromAngle(100, i);
    }
}

void Player::rayCastDDD(Window *window, SDL_Rect *port, Map *map)
{
    HitResult hitResult;
    for (int i = 0; i < MAX_RAYS; i++)
    {
        hitResult = this->rays[i].castDDD(this->pos, this->lookAt, map);

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
