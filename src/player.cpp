#include "player.hpp"

#include "circle.hpp"

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
    SDL_RenderDrawLineF(w->renderer, t.p1.x, t.p1.y, t.p2.x, t.p2.y); //p1 to p2
    SDL_RenderDrawLineF(w->renderer, t.p2.x, t.p2.y, t.p3.x, t.p3.y); // p2 to p3
    SDL_RenderDrawLineF(w->renderer, t.p3.x, t.p3.y, t.p1.x, t.p1.y); // p3 to p1
}

Player::Player() {}

void Player::init(Map *w)
{
    this->pos = vec2(w->w / 2, w->h / 2);
    this->velocity = vec2(0.0f, 0.0f);

    this->velocity_mag = 0.0f;
    this->prevVelocity_mag = 0.0f;

    this->horizontalMaxSpeed = 2.0f;
    this->verticalMaxSpeed = 2.0f;
    this->slowdownRate = 0.3f; // times delta time and final push should be 2f?

    std::vector<Ray> vec(MAX_RAYS, Ray());
    this->rays = vec;

    this->angle = 90.0f;
    this->radius = 6.0f * CONST_PI;

    this->triangle = Triangle(vec2(-2.5f, 2.5f), vec2(0.0f, -5.0f), vec2(2.5f, 2.5f));
}

void Player::updatePos(float acceleration, vec2 mousePos, float dt, SDL_Rect *port)
{
    // update velocity and store current angle
    //this->angle = getAngleFromVectors(this->pos, mousePos, vec2(0.0f, 0.0f));
    
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
        if (this->velocity_mag <= 0.05f)
            this->velocity_mag = 0.0f; // final push
    }
    
    // store last velocity magnitud:
    this->prevVelocity_mag = this->velocity_mag;

    this->velocity.x = sinf(deg2rad(angle)) * this->velocity_mag;
    this->velocity.y = -cosf(deg2rad(angle)) * this->velocity_mag; // screen is upside down, y0 up, y last down

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

    //this->pos.x = clamp(this->pos.x, 10.0f, limitX);
    //this->pos.y = clamp(this->pos.y, 10.0f, limitY);

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
    SDL_SetRenderDrawColor(w->renderer, 255, 255, 51, 1);
    /*Circle c;
    int32_t centerX = (int32_t)(this->pos.x /*+ CELL_SIZE / 2*///);
    /*int32_t centerY = (int32_t)(this->pos.y /*+ CELL_SIZE / 2*///);
    /*c.drawCircle(w->renderer, centerX, centerY, this->radius);*/

    // transformations
    Triangle transTriangle;

    // scale
    float size = 5.0f;
    transTriangle.p1 = scale2Dvec(this->triangle.p1, size);
    transTriangle.p2 = scale2Dvec(this->triangle.p2, size);
    transTriangle.p3 = scale2Dvec(this->triangle.p3, size);

    // rotate
    transTriangle.p1 = rotate2Dvec(transTriangle.p1, this->angle);
    transTriangle.p2 = rotate2Dvec(transTriangle.p2, this->angle);
    transTriangle.p3 = rotate2Dvec(transTriangle.p3, this->angle);

    // translate
    transTriangle.p1 = transTriangle.p1 + this->pos;
    transTriangle.p2 = transTriangle.p2 + this->pos;
    transTriangle.p3 = transTriangle.p3 + this->pos;

    this->translTriangle = transTriangle; // just to debug

    // draw triangle
    drawTriangle(w, transTriangle);

    //draw line from player to triangle
    SDL_RenderDrawLineF(w->renderer, (float)this->pos.x, (float)this->pos.y, transTriangle.p2.x, transTriangle.p2.y);
}

void Player::setUpRays()
{
    int i = 0;
    std::vector<Ray>::iterator it = this->rays.begin();
    std::vector<Ray>::iterator end = this->rays.end();

    for (it; it != end; ++it)
    {
        it->pos = this->pos;
        it->dir = getVecFromAngle(1, this->angle) + getVecFromAngle(100, i);
        i++;
    }
}

void Player::clearRays()
{
    this->rays.clear();
}
